/************************************************************************
 SYMDYN_STOC
 ************************************************************************/

#include <iostream>
#include <random>
#include <iomanip>
#include <fstream> // output to files
#include <cmath> // log function (for natural logarithm)

using namespace std;


int main()
{

   // Initialise variables
   unsigned int N, // N is the host population size
                Nc, // Nc is the number of occupied hosts
                n0, // n0 is the initial symbiont population sizes for the initially occupied hosts
                ne, // ne is the initial number of (free-living) dispersal symbionts
                T, // T is total number of days of the simulated time
                out_tfreq, //  Frequency of days shown in the output
                out_tinit, // Initial day to start the output
                Rep, // Number of repetitions of each scenario
                N_Scen, // Number of scenarios to simulate
                Thread; // It adjusts the output variable Scen when multiple processors are used.

   unsigned seed; // seed number for the random generator;


   bool    full; // Parameter to determine whether the simulations should be stopped when the symbiont becomes extinct.
                 // True = full simulation; False = simulation stopped when symbiont becomes extinct.

   double   M, // M is the per day and per capita host mortality rate
           P, // P is the probability that a died host was due to predation
           b, // b is the per day and per capita symbiont birth rate
           d, // d is the per day and per capita (within-host) symbiont death rate
           C, // C is the density dependence parameter of symbiont populations
           s, // s is the per day and per capita emigration propensity (i.e. dispersal rate)
           m, // m is the per day and per capita mortality rate of dispersal symbionts
           tr; // tr is the per day and per capita transmission efficacy of dispersal symbionts

         // Initialization of N_Scen
         cin >> N_Scen;
         cin >> Thread;
         cin >> seed;

         // Declare random generator and distribution to get random numbers
         std::mt19937 generator;
         std::uniform_real_distribution<double> distribution ( 0.0 , 1.0 );
         generator.seed ( seed ); // For host and symbiont mortality


   // Scenarios to simulate
   for ( unsigned int scenarios = 1; scenarios <= N_Scen; scenarios++ )
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
         cin >> b;
         cin >> d;
         cin >> C;
         cin >> s;
         cin >> m;
         cin >> n0;
         cin >> ne;
         cin >> tr;

         // Variables to reinitialise dynamic parameters after each repetition
         unsigned int Nc_init = Nc;
         unsigned int ne_init = ne;

         // Create an array to store symbiont population sizes (ni)
         unsigned int n [ N ] = {0};

         // Set initial Nc to n0
         for ( unsigned int counter1 = 1; counter1 <= Nc; counter1++ )
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
               << "b = " << b << endl
               << "d = " << d << endl
               << "C = " << C << endl
               << "s = " << s << endl
               << "m = " << m << endl
               << "n0 = " << n0 << endl
               << "ne = " << ne << endl
               << "tr = " << tr << endl
               << "\n\n";

               // Print headers for summary results
               cout << "Event"
               << "," << "Time"
               << "," << "Scen"
               << "," << "Rep"
               << "," << "Day"
               << "," << "Prev"
               << "," << "Ab"
               << "," << "ne"
               << endl;

            }  // Output initial parameter values and headers

         // Repetitions of each scenario
         for ( unsigned int repetitions = 1; repetitions <= Rep; repetitions++ )

            {
               // Reinitialisation of dynamic parameters after each repetition
               if (repetitions > 1)
                  {
                     // Reinitialise Nc and ne after each repetition
                     Nc = Nc_init;
                     ne = ne_init;

                     // Reinitialise array of symbiont population sizes
                     for ( unsigned int i = 1; i <= N; i++ )
                        n [ i - 1 ] = 0;

                     for ( unsigned int i = 1; i <= Nc; i++ )
                        n [ i - 1 ] = n0;
                  }

               // Starting each simulation
               unsigned long int step = 1;
               unsigned int day = 0;
               double  t = 0; // Simulation time (in days) at a given step

               // Create the variable for total rate of all the possible events
               double R = 0;

               // Create the variable for total rates of within-host events
               double Pbtot = 0;
               double Pdtot = 0;
               double Pstot = 0;

               // Create arrays to store rates of each individual population
               double Pb [ N ] = {0};
               double Pd [ N ] = {0};
               double Ps [ N ] = {0};

               // Create variables to store rates of off-host dynamics
               double MN = M * N;
               double mne = m * ne;
               double trN = tr * N;
               double trr = tr * N * ne;

               while ( t < T ) // For each step

                  {

                     if ( step == 1)
                        {
                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 Pb[i] = b * n[i];
                                 Pd[i] = ( d + C * n[i] ) * n[i];
                                 Ps[i] = s * n[i];
                                 Pbtot += Pb[i];
                                 Pdtot += Pd[i];
                                 Pstot += Ps[i];
                              }
                           R = Pbtot + Pdtot + Pstot + MN + mne + trr;
                         }

                     // Update t and day
                     double tinc = - log ( distribution ( generator ) ) / R;
                     t += tinc;

                     if ( t > day + 1 )
                        ++day;

                     // Random number to decide what type of event occurs
                     double x = distribution ( generator ) * R;

                     // Birth
                     if ( x < Pbtot )
                        {
                           double sumbi = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumbi += Pb[i];

                                 if ( x < sumbi )
                                    {
                                       ++n[i];
                                       double Pb_new = b * n[i];
                                       double Pd_new = ( d + C * n[i] ) * n[i];
                                       double Ps_new = s * n[i];
                                       R -= Pb[i] + Pd[i] + Ps[i] ;
                                       Pbtot -= Pb[i];
                                       Pdtot -= Pd[i];
                                       Pstot -= Ps[i];
                                       Pb[i] = Pb_new;
                                       Pd[i] = Pd_new;
                                       Ps[i] = Ps_new;
                                       Pbtot += Pb_new;
                                       Pdtot += Pd_new;
                                       Pstot += Ps_new;
                                       R += Pb_new + Pd_new + Ps_new;
                                       break;
                                    }
                              }
                        } // End birth

                     // Symbiont death on host
                     else if ( x < Pbtot + Pdtot )
                        {
                           double sumdi = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumdi += Pd[i];

                                 if ( x < Pbtot + sumdi )
                                    {
                                       --n[i];
                                       double Pb_new = b * n[i];
                                       double Pd_new = ( d + C * n[i] ) * n[i];
                                       double Ps_new = s * n[i];
                                       R -= Pb[i] + Pd[i] + Ps[i];
                                       Pbtot -= Pb[i];
                                       Pdtot -= Pd[i];
                                       Pstot -= Ps[i];
                                       Pb[i] = Pb_new;
                                       Pd[i] = Pd_new;
                                       Ps[i] = Ps_new;
                                       Pbtot += Pb_new;
                                       Pdtot += Pd_new;
                                       Pstot += Ps_new;
                                       R += Pb_new + Pd_new + Ps_new;
                                       break;
                                    }
                              }
                        } // End death on host

                     // Emigration
                     else if ( x < Pbtot + Pdtot + Pstot )
                        {
                           double sumsi = 0;

                           for ( unsigned int i = 0; i <= ( N - 1); i++ )
                              {
                                 sumsi += Ps[i];

                                 if ( x < Pbtot + Pdtot + sumsi )
                                    {
                                       --n[i];
                                       double Pb_new = b * n[i];
                                       double Pd_new = ( d + C * n[i] ) * n[i];
                                       double Ps_new = s * n[i];
                                       R -= Pb[i] + Pd[i] + Ps[i];
                                       Pbtot -= Pb[i];
                                       Pdtot -= Pd[i];
                                       Pstot -= Ps[i];
                                       Pb[i] = Pb_new;
                                       Pd[i] = Pd_new;
                                       Ps[i] = Ps_new;
                                       Pbtot += Pb_new;
                                       Pdtot += Pd_new;
                                       Pstot += Ps_new;
                                       R += Pb_new + Pd_new + Ps_new;
                                       R -= m * ne + trN * ne;
                                       ++ne;
                                       R += m * ne + trN * ne;
                                       break;
                                    }
                              }
                        } // End emigration


                     // Host death
                     else if ( x < Pbtot + Pdtot + Pstot + MN )
                        {
                           int y = static_cast<int> ( distribution ( generator ) * N );

                           if ( distribution ( generator ) > P )
                              {

                                 R -= m * ne + trN * ne;
                                 ne += n[y];
                                 R += m * ne + trN * ne;
                              }

                           n[y] = 0;
                           R -= Pb[y] + Pd[y] + Ps[y];
                           Pbtot -= Pb[y];
                           Pdtot -= Pd[y];
                           Pstot -= Ps[y];
                           Pb[y] = 0;
                           Pd[y] = 0;
                           Ps[y] = 0;

                        } // End host death


                     // Death of a dispersal symbiont
                     else if ( x < Pbtot + Pdtot + Pstot + MN + m*ne )
                        {
                           R -= m * ne + trN * ne;
                           --ne;
                           R += m * ne + trN * ne;

                        } // End of death of dispersal symbiont


                     // Transmission of a symbiont
                     else  // if ( x < Pb + Pd + Ps + MN + mne +  trr )
                        {
                           R -= m * ne + trN * ne;
                           --ne;
                           R += m * ne + trN * ne;

                           int y = static_cast<int> ( distribution ( generator ) * N );
                           ++n[y];
                           double Pb_new = b * n[y];
                           double Pd_new = ( d + C * n[y] ) * n[y];
                           double Ps_new = s * n[y];
                           R -= Pb[y] + Pd[y] + Ps[y] ;
                           Pbtot -= Pb[y];
                           Pdtot -= Pd[y];
                           Pstot -= Ps[y];
                           Pb[y] = Pb_new;
                           Pd[y] = Pd_new;
                           Ps[y] = Ps_new;
                           Pbtot += Pb_new;
                           Pdtot += Pd_new;
                           Pstot += Ps_new;
                           R += Pb_new + Pd_new + Ps_new;
                        } // end of transmission of a symbiont




                     // Limitation of output to a frequency of "out_tfreq" days starting from the day "out_tinit"
                     // In addition, the day 1 is always shown by the output
                     if ( t - tinc < day )
                        {

                           // To finish when symbionts become extinct
                           if ( !full && Nc == 0 && ne < 1 )
                              {

                                 // Obtaining symbiont population parameters (Prev and Ab)
                                 // Prev is the percentage of occupied hosts
                                 // Ab is the mean symbiont population size of occupied hosts

                                 Nc = 0; // Update Nc

                                 for ( unsigned int counter4 = 0; counter4 <= ( N - 1 ); counter4++ ) // For each host
                                    {
                                       if ( n [ counter4 ] > 0 )
                                          ++Nc;
                                    }

                                 float Prev = static_cast<float> (Nc) / N; // Calculate prevalence

                                 float Ab = 0; // Calculate mean Abundande in occupied hosts

                                 for ( unsigned int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
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

                                       // Output
                                       cout << step
                                       << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << t
                                       << "," << scenarios + Thread
                                       << "," << repetitions
                                       << "," << day
                                       << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev
                                       << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab
                                       << "," << ne << endl;
  
                                 break;
                              }
                           else if ( day % out_tfreq == 0 && day - ( out_tinit - 1 ) > 0 )
                              {

                                  // Obtaining symbiont population parameters (Prev and Ab)
                                  // Prev is the percentage of occupied hosts
                                  // Ab is the mean symbiont population size of occupied hosts

                                  Nc = 0; // Update Nc

                                  for ( unsigned int counter4 = 0; counter4 <= ( N - 1 ); counter4++ ) // For each host
                                     {
                                        if ( n [ counter4 ] > 0 )
                                           ++Nc;
                                     }

                                  float Prev = static_cast<float> (Nc) / N; // Calculate prevalence

                                  float Ab = 0; // Calculate mean Abundande in occupied hosts

                                  for ( unsigned int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
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

                                 // Output
                                 cout << step
                                 << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << t
                                 << "," << scenarios + Thread
                                 << "," << repetitions
                                 << "," << day
                                 << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev
                                 << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab
                                 << "," << ne << endl;
                              }

                        }

                     ++step;


                  } // End for each step
                    // Ending each simulation

            } // End of repetitions

      } // Ending scenarios to simulate

    return 0;
}
