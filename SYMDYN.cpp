/************************************************************************
 SYMDYN
 ************************************************************************/

#include <iostream>
#include <random>
#include <iomanip>
#include <fstream> // output to files
#include <cstdlib> // srand, rand


using namespace std;


int main()
{

   // Initialise variables
   int N, // N is the host population size
       Nc, // Nc is the number of occupied hosts
       T, // T is total number of days of the simulated time
       out_tfreq, //  Frequency of days shown in the output
       out_tinit, // Initial day to start the output
       Rep, // Number of repetitions of each scenario
       N_Scen, // Number of scenarios to simulate
       Thread; // It adjusts the output variable Scen when multiple processors are used.

   unsigned seed; // seed for the random generator

   bool    full; // Parameter to determine whether the simulations should be stopped when the symbiont becomes extinct.
                 // True = full simulation; False = simulation stopped when the symbiont becomes extinct.

   float   M, // M is the per day and per capita host mortality rate
           P, // P is the probability that a dead host was predated (leading to the death of its symbionts)
           r, // r is the per day and per capita intrinsic symbiont population growth
           C, // C is the density dependent parameter of symbiont populations
           s, // s is the per day and per capita emigration propensity (i.e. dispersal rate)
           n0, // n0 is the initial symbiont population sizes for the initially occupied hosts
           ne, // ne is the number of the (free-living) dispersal symbionts
           m, // m is the per day and per capita mortality rate of dispersal symbionts
           t, // t is the per day and per capita transmission efficacy (i.e. immigration rate) of dispersal symbionts
           h; //h is the duration of each running time step of the model


         // Initialisation of N_Scen
         cin >> N_Scen;
         cin >> Thread;
         cin >> seed;


         // Declare random generators to get random numbers
         std::mt19937 generator;
         generator.seed ( seed ); // For host and symbiont mortality


   // Scenarios to simulate
   for ( int scenarios = 1; scenarios <= N_Scen; scenarios++ )
      {

         // Input parameter values
         cin >> N;
         cin >> Nc;
         cin >> T;
         cin >> out_tfreq;
         cin >> out_tinit;
         cin >> Rep;
         cin >> full;
         cin >> M;
         cin >> P;
         cin >> r;
         cin >> C;
         cin >> s;
         cin >> m;
         cin >> n0;
         cin >> ne;
         cin >> t;
         cin >> h;

         // Variables to reinitialise dynamic parameters after each repetition
         int Nc_init = Nc;
         float ne_init = ne;


         // Create an array to store symbiont population sizes (ni)
         float n [ N ] = {0};


         // Set initial Nc to n0
         for ( int counter1 = 1; counter1 <= Nc; counter1++ )
            n [ counter1 - 1 ] = n0;


         if ( scenarios == 1 ) // Output initial parameter values and headers
            {

               // Print initial parameter values
               cout << "N_Scen = " << N_Scen << endl
               << "Thread = " << Thread << endl
               << "Seed = " << seed << endl
               << "N = " << N << endl
               << "Nc = " << Nc << endl
               << "T = " << T << endl
               << "out_tfreq = " << out_tfreq << endl
               << "out_tinit = " << out_tinit << endl
               << "Rep = " << Rep << endl
               << "full = " << full << endl
               << "M = " << M << endl
               << "P = " << P << endl
               << "r = " << r << endl
               << "C = " << C << endl
               << "s = " << s << endl
               << "m = " << m << endl
               << "n0 = " << n0 << endl
               << "ne = " << ne << endl
               << "t = " << t << endl
               << "h = " << h << endl
               << "\n\n";

               // Print headers for summary results
               cout << "Scen"
               << "," << "Rep"
               << "," << "Day"
               << "," << "Prev"
               << "," << "Ab"
               << "," << "ne"
               << endl;
            }  // Output initial parameter values and headers


         // Repetitions of each scenario
         for ( int repetitions = 1; repetitions <= Rep; repetitions++ )

            {


               // Reinitialisation of dynamic parameters after each repetition
               if (repetitions > 1)
                  {
                     // Reinitialise Nc and ne after each repetition
                     Nc = Nc_init;
                     ne = ne_init;

                     // Reinitialise array of symbiont population sizes
                     for ( int counter1 = 1; counter1 <= N; counter1++ )
                     n [ counter1 - 1 ] = 0;

                     for ( int counter1 = 1; counter1 <= Nc; counter1++ )
                     n [ counter1 - 1 ] = n0;
                  }


               // Starting each simulation
               for ( int step = 1; step <= T / h; step++ ) // For each time step

                  {
                      for ( int counter3 = 0; counter3 <= ( N - 1 ); counter3++ ) // For each host
                         {
                            if ( n [ counter3 ] > 0 ) // For occupied hosts
                               {
                                 // Update ni by adding growth and removing emigration at each step (mechanistically)
                                   float ni = n [ counter3 ],
                                         ni1,
                                         ni2;

                                   ni1 = ni + ( ni * ( r - C * ni ) - s * ni ) * (h / 2);
                                   ni2 = ni + ( ni1 * ( r - C * ni1 ) - s * ni1 ) * h;
                                   n [ counter3 ] = ni2;

                                   // Decide if the host dies or survive at his step
                                   std::bernoulli_distribution bernoulli_M(M * h);

                                   if ( bernoulli_M ( generator ) )
                                      // Host die
                                      {
                                         n [ counter3 ] = 0;
                                         std::bernoulli_distribution bernoulli_P(P);

                                         if ( bernoulli_P ( generator ) == false)
                                            // Host is not predated
                                            // Update ne by adding released symbionts from host death
                                            {
                                            ne += ni1;
                                            }
                                      }
                                   else
                                      // Host survives
                                      // Update ne by adding emigration of the alive host
                                      {
                                         ne += s * ni1 * h;
                                      }
                               } // End for occupied hosts
                         } // End for each host


                     // Remove random number of dispersal symbionts from ne due to mortality
                     // Random number based on a poisson distribution with mean = m * ne
                     int ne_m = static_cast<int> ( ne );
                     std::binomial_distribution<int> binomial_m( ne_m , m * h );

                     int prand_number = binomial_m ( generator );

                     if ( prand_number < ne )
                        {
                           ne -= prand_number;
                        }
                     else
                        {
                            ne = 0;
                        }


                     // Transmitted symbionts
                     int ne_t = static_cast<int> ( ne );
                     std::binomial_distribution<int> binomial_d( ne_t, t * h * N );
                     int nt = static_cast<int> ( binomial_d ( generator ) ); // Calculate number of transmitted symbionts

                     if ( nt < ne ) // Update ne
                        {
                           ne -= nt;
                        }
                     else
                        {
                           ne = 0;
                        }

                     for ( int i = 1; i <= nt; i++ ) // Distribute randomly the transmitted symbionts to hosts
                        {
                           std::uniform_int_distribution<int> rUnif(0,N);
                           ++n [ rUnif ( generator ) ];
                        }


                     // Obtaining symbiont population parameters (Prev and Ab)
                     // Prev is the percentage of occupied hosts
                     // Ab is the mean symbiont population size of occupied hosts

                     Nc = 0; // Update Nc

                     for ( int counter4 = 0; counter4 <= ( N - 1 ); counter4++ ) // For each host
                        {
                           if ( n [ counter4 ] > 0 )
                              ++Nc;
                        }

                     float Prev = static_cast<float> (Nc) / N; // Calculate prevalence

                     float Ab = 0; // Calculate mean Abundande in occupied hosts

                     for ( int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
                        {
                           if ( n [ counter5 ] > 0 )
                           {
                              Ab += n [ counter5 ];
                           }
                        }

                     if ( Nc > 0 )
                        {
                           Ab = Ab / Nc;
                        }

                     // Limitation of output to a frequency of "out_tfreq" days starting from the day "out_tinit"
                     // In addition, the day 1 is always shown by the output
                     if ( step % static_cast<int>( out_tfreq / h ) == 0 && step - ( ( out_tinit - 1 ) / h ) > 0 ) // Limitation of output
                        {
                           // Output
                           cout << scenarios + Thread
                           << "," << repetitions
                           << "," << static_cast<int> (step * h)
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab
                           << "," << ne << endl;
                        }


                     // To finish when symbionts are extinct
                     if ( !full && Nc == 0 && ne < 1 )
                        {
                           // Output
                           cout << scenarios + Thread
                           << "," << repetitions
                           << "," << static_cast<int> (step * h)
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab
                           << "," << ne << endl;

                           break;
                        }


                  } // End for each time step
                    // Ending each simulation

            } // End of repetitions

      } // Ending scenarios to simulate

    return 0;
}
