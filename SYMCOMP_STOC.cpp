/************************************************************************
 SYMCOMP_STOC
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

                Nc[10], // Nc stores the number of initially occupied hosts for each of all the 10 clones
                n0[10], // n0 stores the initial symbiont population sizes for each of all the 10 clones
                ne[10], // ne stores the initial number of dispersal symbionts of the ten clones
                T, // T is total number of days of the simulated time
                out_tfreq, //  Frequency of days shown in the output
                out_tinit, // Initial day to start the output
                Rep, // Number of repetitions of each scenario
                N_Scen, // Number of scenarios to be simulated
                Thread; // It adjusts the output variable Scen when multiple processors are used.

   unsigned seed; // seed number for the random generator;


   bool    full, // Parameter to determine whether the simulations should be stopped when the symbionts become extinct.
                 // True = full simulation; False = simulation stopped when symbionts become extinct.
           Nc_sharing; //  Parameter to determine whether the initial symbionts (n0) either share or use different occupied hosts.

   double   M, // M is the per day and per capita host mortality rate
           P, // P is the probability that a died host was due to predation, i.e. leading to the death of its symbionts

           // Clone 1
           b[10], // b stores the per day and per capita birth rate of all the 10 clones
           d[10], // d stores the per day and per capita (within-host) death rate of all the 10 clones
           C[10], // C stores the density dependence parameter of populations of all the 10 clones
           s[10], // s stores the per day and per capita emigration propensity (i.e. dispersal rate) of clone all the 10 clones
           m[10], // m stores the per day and per capita mortality rate of (free-living) dispersal symbionts of all the 10 clones
           tr[10]; // t stores the per day and per capita transmission efficacy of dispersal symbionts of all the 10 clones

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
         for ( int j = 0; j <= ( 10 - 1 ); j++ )
            { cin >> Nc[j]; }
         cin >> T;
         cin >> out_tfreq;
         cin >> out_tinit;
         cin >> Rep;
         cin >> full;
         cin >> Nc_sharing;
         cin >> M;
         cin >> P;

         for ( int j = 0; j <= ( 10 - 1 ); j++ )
            { cin >> b[j];
              cin >> d[j];
              cin >> C[j];
              cin >> s[j];
              cin >> m[j];
              cin >> n0[j];
              cin >> ne[j];
              cin >> tr[j];
            }

         // Variables to reinitialise dynamic parameters after each repetition
         unsigned int Nc_init[10];
         unsigned int ne_init[10];

         for ( int j = 0; j <= ( 10 - 1 ); j++ )
            {
               Nc_init[j] = Nc[j];
               ne_init[j] = ne[j];
            }

         if ( scenarios == 1 ) // Output initial parameter values and headers
            {
               // Print initial parameter values
               cout << "N_Scen = " << N_Scen << endl
               << "Thread = " << Thread << endl
               << "Seed = " << seed << endl
               << "N = " << N << endl;
               for ( int j = 0; j <= ( 10 - 1 ); j++ )
                  { cout << "Nc" << j + 1 << " = " << Nc[j] << endl; }
               cout << "T = " << T << endl
               << "out_tfreq = " << out_tfreq << endl
               << "out_tinit = " << out_tinit << endl
               << "Rep = " << Rep << endl
               << "full = " << full << endl
               << "Nc_sharing = " << Nc_sharing << endl
               << "M = " << M << endl
               << "P = " << P << endl;

               for ( int j = 0; j <= ( 10 - 1 ); j++ )
                  { cout << "b" << j + 1 << " = " << b[j]  << endl
                    << "d" << j + 1 << " = " << d[j]  << endl
                    << "C" << j + 1 << " = " << C[j]  << endl
                    << "s" << j + 1 << " = " << s[j]  << endl
                    << "n0" << j + 1 << " = " << n0[j]  << endl
                    << "ne" << j + 1 << " = " << ne[j]  << endl
                    << "m" << j + 1 << " = " << m[j]  << endl
                    << "t" << j + 1 << " = " << tr[j]  << endl;
                  }
               cout << "\n\n";

               // Print headers for summary results
               cout << "Event"
               << "," << "Time"
               << "," << "Scen"
               << "," << "Rep"
               << "," << "Day";

               for ( int j = 1; j <= ( 10 ); j++ )
                  {
                     cout << "," << "Prev" << j
                     << "," << "Ab" << j
                     << "," << "VMR" << j
                     << "," << "ne" << j;
                  }
                  cout << endl;
            }  // Output initial parameter values and headers


         // Repetitions of each scenario
         for ( unsigned int repetitions = 1; repetitions <= Rep; repetitions++ )

            {
               // Reinitialise of Nc and ne after each repetition
               for ( int j = 0; j <= ( 10 - 1 ); j++ )
                  {
                     Nc[j] = Nc_init[j];
                     ne[j] = ne_init[j];
                  }

               // Create a two dimensional array to store symbiont population sizes (ni) of all the ten clones
               unsigned int n[10][N] = {0};

               // Set initial Nc to n0 for clone 0
               for ( unsigned int counter1 = 1; counter1 <= Nc[0]; counter1++ ) // Nc clone 0 = Nc[0]
                  n[0] [ counter1 - 1 ] = n0[0];

               // Set initial Nc to n0 for the rest of clones
               if (Nc_sharing) // initially occupied host are shared
                  {
                     for ( int j = 1; j <= ( 10 - 1 ); j++ ) // Comment: we start from 1 because clone 0 was initialised before
                        {
                           for ( int counter = 1; counter <= Nc[j]; counter++ ) // Nc clone 1 = Nc[1]
                              {
                                 n[j] [ counter - 1 ] = n0[j] ;
                              }
                        }
                  }

               else // initially occupied hosts are different for the ten clones
                  {
                     for ( int counter2 = Nc[0] + 1; counter2 <= Nc[0] + Nc[1] ; counter2++ )
                        {
                           n[1] [ counter2 - 1 ] = n0[1];
                        }
                     for ( int counter3 = Nc[0] + Nc[1] + 1; counter3 <= Nc[0] + Nc[1] + Nc[2] ; counter3++ )
                        {
                           n[2] [ counter3 - 1 ] = n0[2];
                        }
                     for ( int counter4 = Nc[0] + Nc[1] + Nc[2] + 1; counter4 <= Nc[0] + Nc[1] + Nc[2] + Nc[3] ; counter4++ )
                        {
                           n[3] [ counter4 - 1 ] = n0[3];
                        }
                     for ( int counter5 = Nc[0] + Nc[1] + Nc[2] + Nc[3] + 1; counter5 <= Nc[0] + Nc[1] + Nc[2] + Nc[3] + Nc[4] ; counter5++ )
                        {
                           n[4] [ counter5 - 1 ] = n0[4];
                        }
                     for ( int counter6 = Nc[0] + Nc[1] + Nc[2] + Nc[3] + Nc[4] + 1; counter6 <= Nc[0] + Nc[1] + Nc[2] + Nc[3] + Nc[4] + Nc[5] ; counter6++ )
                        {
                           n[5] [ counter6 - 1 ] = n0[5];
                        }
                     for ( int counter7 = Nc[0] + Nc[1] + Nc[2] + Nc[3] + Nc[4] + Nc[5] + 1; counter7 <= Nc[0] + Nc[1] + Nc[2] + Nc[3] + Nc[4] + Nc[5] + Nc[6] ; counter7++ )
                        {
                           n[6] [ counter7 - 1 ] = n0[6];
                        }
                     for ( int counter8 = Nc[0] + Nc[1] + Nc[2] + Nc[3] + Nc[4] + Nc[5] + Nc[6] + 1; counter8 <= Nc[0] + Nc[1] + Nc[2] + Nc[3] + Nc[4] + Nc[5] + Nc[6] + Nc[7] ; counter8++ )
                        {
                           n[7] [ counter8 - 1 ] = n0[7];
                        }
                     for ( int counter9 = Nc[0]+Nc[1]+Nc[2]+Nc[3]+Nc[4]+Nc[5]+Nc[6]+Nc[7]+1; counter9 <= Nc[0]+Nc[1]+Nc[2]+Nc[3]+Nc[4]+Nc[5]+Nc[6]+Nc[7]+Nc[8]; counter9++ )
                        {
                           n[8] [ counter9 - 1 ] = n0[8];
                        }
                     for ( int counter10 = Nc[0]+Nc[1]+Nc[2]+Nc[3]+Nc[4]+Nc[5]+Nc[6]+Nc[7]+Nc[8]+1; counter10 <= Nc[0]+Nc[1]+Nc[2]+Nc[3]+Nc[4]+Nc[5]+Nc[6]+Nc[7]+Nc[8]+Nc[9]; counter10++ )
                        {
                           n[9] [ counter10 - 1 ] = n0[9];
                        }
                  }

               // Calculate the total number of symbionts per host:
               unsigned int ntot[N] = {0};

               for ( int i = 0; i <= ( N - 1 ); i++ )
                  {
                      for ( int j = 0; j <= ( 10 - 1 ); j++ )
                         {
                             ntot[i] += n[j][i];
                         }
                  }

               // Starting each simulation
               unsigned long int step = 1;
               unsigned int day = 0;
               double  time = 0; // Simulation time (in days) at a given step

               // Create the variable for total rate of all the possible events
               double R = 0;

               // Create the variable for total rates of within-host events
               double Pbtot[10] = {0};
               double Pdtot[10] = {0};
               double Pstot[10] = {0};

               // Create arrays to store rates of each individual population
               double Pb[10][N] = {0};
               double Pd[10][N] = {0};
               double Ps[10][N] = {0};

               // Create variables to store rates of off-host dynamics
               double MN = M * N;
               double trN[10] = {0};

               for ( int j = 0; j <= ( 10 - 1 ); j++ )
                  {
                     trN[j] = tr[j] * N;
                  }

               while ( time < T ) // For each step

                  {

                     if ( step == 1)
                        {

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                    {
                                        Pb[j][i] = b[j] * n[j][i];
                                        Pd[j][i] =  ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                        Ps[j][i] = s[j] * n[j][i];
                                        Pbtot[j] += Pb[j][i] ;
                                        Pdtot[j] += Pd[j][i] ;
                                        Pstot[j] += Ps[j][i] ;
                                     }
                              }

                        for ( int j = 0; j <= ( 10 - 1 ); j++ )
                           {
                              R += Pbtot[j] + Pdtot[j] + Pstot[j] + m[j] * ne[j] + trN[j] * ne[j];
                           }
                        R += MN;
                        }

                     // Update time and day
                     double timeinc = - log ( distribution ( generator ) ) / R;
                     time += timeinc;

                     if ( time > day + 1 )
                        ++day;

                     // Random number to decide what type of event to occurs
                     double x = distribution ( generator ) * R;

                     // Birth of clone 0 (index 0!!)
                     if ( x < Pbtot[0] )
                        {
                           double sumb0i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumb0i += Pb[0][i];

                                 if ( x < sumb0i )
                                    {
                                       // birth
                                       ++n[0][i];
                                       ++ntot[i];

                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[0] * n[0][i];
                                       Pbtot[0] -= Pb[0][i];
                                       R -= Pb[0][i];
                                       Pb[0][i] = Pb_new;
                                       Pbtot[0] += Pb[0][i];
                                       R += Pb[0][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[0] * n[0][i];
                                       Pstot[0] -= Ps[0][i];
                                       R -= Ps[0][i];
                                       Ps[0][i] = Ps_new;
                                       Pstot[0] += Ps[0][i];
                                       R += Ps[0][i];
                                       break;
                                    }
                              }
                        } // End birth of clone 0

                     // Symbiont death on host (clone 0)
                     else if ( x < Pbtot[0] + Pdtot[0] )
                        {
                           double sumd0i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumd0i += Pd[0][i];

                                 if ( x < Pbtot[0] + sumd0i )
                                    {
                                       --n[0][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[0] * n[0][i];
                                       Pbtot[0] -= Pb[0][i];
                                       R -= Pb[0][i];
                                       Pb[0][i] = Pb_new;
                                       Pbtot[0] += Pb[0][i];
                                       R += Pb[0][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[0] * n[0][i];
                                       Pstot[0] -= Ps[0][i];
                                       R -= Ps[0][i];
                                       Ps[0][i] = Ps_new;
                                       Pstot[0] += Ps[0][i];
                                       R += Ps[0][i];
                                       break;
                                    }
                              }
                        } // End death on host (clone 0)

                     // emigration of clone 0
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] )
                        {
                           double sums0i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1); i++ )
                              {
                                 sums0i += Ps[0][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + sums0i )
                                    {
                                       --n[0][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[0] * n[0][i];
                                       Pbtot[0] -= Pb[0][i];
                                       R -= Pb[0][i];
                                       Pb[0][i] = Pb_new;
                                       Pbtot[0] += Pb[0][i];
                                       R += Pb[0][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[0] * n[0][i];
                                       Pstot[0] -= Ps[0][i];
                                       R -= Ps[0][i];
                                       Ps[0][i] = Ps_new;
                                       Pstot[0] += Ps[0][i];
                                       R += Ps[0][i];

                                       // Updating off-host rates of target clone
                                       R -= m[0] * ne[0] + trN[0] * ne[0];
                                       ++ne[0];
                                       R += m[0] * ne[0] + trN[0] * ne[0];
                                       break;
                                    }
                              }
                        } // End emigration of clone 0

                     // Birth of clone 1
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] )
                        {
                           double sumb1i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumb1i += Pb[1][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + sumb1i )
                                    {
                                       ++n[1][i];
                                       ++ntot[i];

                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[1] * n[1][i];
                                       Pbtot[1] -= Pb[1][i];
                                       R -= Pb[1][i];
                                       Pb[1][i] = Pb_new;
                                       Pbtot[1] += Pb[1][i];
                                       R += Pb[1][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[1] * n[1][i];
                                       Pstot[1] -= Ps[1][i];
                                       R -= Ps[1][i];
                                       Ps[1][i] = Ps_new;
                                       Pstot[1] += Ps[1][i];
                                       R += Ps[1][i];
                                       break;
                                    }
                              }
                        } // End birth of clone 1

                     // Symbiont death on host (clone 1)
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] )
                        {
                           double sumd1i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumd1i += Pd[1][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + sumd1i )
                                    {
                                       --n[1][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[1] * n[1][i];
                                       Pbtot[1] -= Pb[1][i];
                                       R -= Pb[1][i];
                                       Pb[1][i] = Pb_new;
                                       Pbtot[1] += Pb[1][i];
                                       R += Pb[1][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[1] * n[1][i];
                                       Pstot[1] -= Ps[1][i];
                                       R -= Ps[1][i];
                                       Ps[1][i] = Ps_new;
                                       Pstot[1] += Ps[1][i];
                                       R += Ps[1][i];
                                       break;
                                    }
                              }
                        } // End death on host (clone 1)

                     // Emigration of clone 1
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] )
                        {
                           double sums1i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1); i++ )
                              {
                                 sums1i += Ps[1][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + sums1i )
                                    {
                                       --n[1][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[1] * n[1][i];
                                       Pbtot[1] -= Pb[1][i];
                                       R -= Pb[1][i];
                                       Pb[1][i] = Pb_new;
                                       Pbtot[1] += Pb[1][i];
                                       R += Pb[1][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[1] * n[1][i];
                                       Pstot[1] -= Ps[1][i];
                                       R -= Ps[1][i];
                                       Ps[1][i] = Ps_new;
                                       Pstot[1] += Ps[1][i];
                                       R += Ps[1][i];

                                       // Updating off-host rates of target clone
                                       R -= m[1] * ne[1] + trN[1] * ne[1];
                                       ++ne[1];
                                       R += m[1] * ne[1] + trN[1] * ne[1];
                                       break;
                                    }
                              }
                        } // End emigration of clone 1

                    // Birth of clone 2
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] )
                        {
                           double sumb2i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumb2i += Pb[2][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + sumb2i )
                                    {
                                       ++n[2][i];
                                       ++ntot[i];

                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[2] * n[2][i];
                                       Pbtot[2] -= Pb[2][i];
                                       R -= Pb[2][i];
                                       Pb[2][i] = Pb_new;
                                       Pbtot[2] += Pb[2][i];
                                       R += Pb[2][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[2] * n[2][i];
                                       Pstot[2] -= Ps[2][i];
                                       R -= Ps[2][i];
                                       Ps[2][i] = Ps_new;
                                       Pstot[2] += Ps[2][i];
                                       R += Ps[2][i];
                                       break;
                                    }
                              }
                        } // End birth of clone 2

                     // Symbiont death on host (clone 2)
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] )
                        {
                           double sumd2i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumd2i += Pd[2][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + sumd2i )
                                    {
                                       --n[2][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[2] * n[2][i];
                                       Pbtot[2] -= Pb[2][i];
                                       R -= Pb[2][i];
                                       Pb[2][i] = Pb_new;
                                       Pbtot[2] += Pb[2][i];
                                       R += Pb[2][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[2] * n[2][i];
                                       Pstot[2] -= Ps[2][i];
                                       R -= Ps[2][i];
                                       Ps[2][i] = Ps_new;
                                       Pstot[2] += Ps[2][i];
                                       R += Ps[2][i];
                                       break;
                                    }
                              }
                        } // End death on host (clone 2)

                     // Emigration of clone 2
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] )
                        {
                           double sums2i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1); i++ )
                              {
                                 sums2i += Ps[2][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + sums2i )
                                    {
                                       --n[2][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[2] * n[2][i];
                                       Pbtot[2] -= Pb[2][i];
                                       R -= Pb[2][i];
                                       Pb[2][i] = Pb_new;
                                       Pbtot[2] += Pb[2][i];
                                       R += Pb[2][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[2] * n[2][i];
                                       Pstot[2] -= Ps[2][i];
                                       R -= Ps[2][i];
                                       Ps[2][i] = Ps_new;
                                       Pstot[2] += Ps[2][i];
                                       R += Ps[2][i];

                                       // Updating off-host rates of target clone
                                       R -= m[2] * ne[2] + trN[2] * ne[2];
                                       ++ne[2];
                                       R += m[2] * ne[2] + trN[2] * ne[2];
                                       break;
                                    }
                              }
                        } // End emigration of clone 2

                    // Birth of clone 3
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] )
                        {
                           double sumb3i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumb3i += Pb[3][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + sumb3i )
                                    {
                                       ++n[3][i];
                                       ++ntot[i];

                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[3] * n[3][i];
                                       Pbtot[3] -= Pb[3][i];
                                       R -= Pb[3][i];
                                       Pb[3][i] = Pb_new;
                                       Pbtot[3] += Pb[3][i];
                                       R += Pb[3][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[3] * n[3][i];
                                       Pstot[3] -= Ps[3][i];
                                       R -= Ps[3][i];
                                       Ps[3][i] = Ps_new;
                                       Pstot[3] += Ps[3][i];
                                       R += Ps[3][i];
                                       break;
                                    }
                              }
                        } // End birth of clone 3

                     // Symbiont death on host (clone 3)
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] )
                        {
                           double sumd3i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumd3i += Pd[3][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + sumd3i )
                                    {
                                       --n[3][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[3] * n[3][i];
                                       Pbtot[3] -= Pb[3][i];
                                       R -= Pb[3][i];
                                       Pb[3][i] = Pb_new;
                                       Pbtot[3] += Pb[3][i];
                                       R += Pb[3][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[3] * n[3][i];
                                       Pstot[3] -= Ps[3][i];
                                       R -= Ps[3][i];
                                       Ps[3][i] = Ps_new;
                                       Pstot[3] += Ps[3][i];
                                       R += Ps[3][i];
                                       break;
                                    }
                              }
                        } // End death on host (clone 3)

                     // Emigration of clone 3
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] )
                        {
                           double sums3i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1); i++ )
                              {
                                 sums3i += Ps[3][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + sums3i )
                                    {
                                       --n[3][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[3] * n[3][i];
                                       Pbtot[3] -= Pb[3][i];
                                       R -= Pb[3][i];
                                       Pb[3][i] = Pb_new;
                                       Pbtot[3] += Pb[3][i];
                                       R += Pb[3][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[3] * n[3][i];
                                       Pstot[3] -= Ps[3][i];
                                       R -= Ps[3][i];
                                       Ps[3][i] = Ps_new;
                                       Pstot[3] += Ps[3][i];
                                       R += Ps[3][i];

                                       // Updating off-host rates of target clone
                                       R -= m[3] * ne[3] + trN[3] * ne[3];
                                       ++ne[3];
                                       R += m[3] * ne[3] + trN[3] * ne[3];
                                       break;
                                    }
                              }
                        } // End emigration of clone 3

                    // Birth of clone 4
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] )
                        {
                           double sumb4i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumb4i += Pb[4][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + sumb4i )
                                    {
                                       ++n[4][i];
                                       ++ntot[i];

                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[4] * n[4][i];
                                       Pbtot[4] -= Pb[4][i];
                                       R -= Pb[4][i];
                                       Pb[4][i] = Pb_new;
                                       Pbtot[4] += Pb[4][i];
                                       R += Pb[4][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[4] * n[4][i];
                                       Pstot[4] -= Ps[4][i];
                                       R -= Ps[4][i];
                                       Ps[4][i] = Ps_new;
                                       Pstot[4] += Ps[4][i];
                                       R += Ps[4][i];
                                       break;
                                    }
                              }
                        } // End birth of clone 4

                     // Symbiont death on host (clone 4)
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] )
                        {
                           double sumd4i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumd4i += Pd[4][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + sumd4i )
                                    {
                                       --n[4][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[4] * n[4][i];
                                       Pbtot[4] -= Pb[4][i];
                                       R -= Pb[4][i];
                                       Pb[4][i] = Pb_new;
                                       Pbtot[4] += Pb[4][i];
                                       R += Pb[4][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[4] * n[4][i];
                                       Pstot[4] -= Ps[4][i];
                                       R -= Ps[4][i];
                                       Ps[4][i] = Ps_new;
                                       Pstot[4] += Ps[4][i];
                                       R += Ps[4][i];
                                       break;
                                    }
                              }
                        } // End death on host (clone 4)

                     // Emigration of clone 4
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] )
                        {
                           double sums4i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1); i++ )
                              {
                                 sums4i += Ps[4][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + sums4i )
                                    {
                                       --n[4][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[4] * n[4][i];
                                       Pbtot[4] -= Pb[4][i];
                                       R -= Pb[4][i];
                                       Pb[4][i] = Pb_new;
                                       Pbtot[4] += Pb[4][i];
                                       R += Pb[4][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[4] * n[4][i];
                                       Pstot[4] -= Ps[4][i];
                                       R -= Ps[4][i];
                                       Ps[4][i] = Ps_new;
                                       Pstot[4] += Ps[4][i];
                                       R += Ps[4][i];

                                       // Updating off-host rates of target clone
                                       R -= m[4] * ne[4] + trN[4] * ne[4];
                                       ++ne[4];
                                       R += m[4] * ne[4] + trN[4] * ne[4];
                                       break;
                                    }
                              }
                        } // End emigration of clone 4

                    // Birth of clone 5
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] )
                        {
                           double sumb5i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumb5i += Pb[5][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + sumb5i )
                                    {
                                       ++n[5][i];
                                       ++ntot[i];

                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[5] * n[5][i];
                                       Pbtot[5] -= Pb[5][i];
                                       R -= Pb[5][i];
                                       Pb[5][i] = Pb_new;
                                       Pbtot[5] += Pb[5][i];
                                       R += Pb[5][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[5] * n[5][i];
                                       Pstot[5] -= Ps[5][i];
                                       R -= Ps[5][i];
                                       Ps[5][i] = Ps_new;
                                       Pstot[5] += Ps[5][i];
                                       R += Ps[5][i];
                                       break;
                                    }
                              }
                        } // End birth of clone 5

                     // Symbiont death on host (clone 5)
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] )
                        {
                           double sumd5i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumd5i += Pd[5][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + sumd5i )
                                    {
                                       --n[5][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[5] * n[5][i];
                                       Pbtot[5] -= Pb[5][i];
                                       R -= Pb[5][i];
                                       Pb[5][i] = Pb_new;
                                       Pbtot[5] += Pb[5][i];
                                       R += Pb[5][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[5] * n[5][i];
                                       Pstot[5] -= Ps[5][i];
                                       R -= Ps[5][i];
                                       Ps[5][i] = Ps_new;
                                       Pstot[5] += Ps[5][i];
                                       R += Ps[5][i];
                                       break;
                                    }
                              }
                        } // End death on host (clone 5)

                     // Emigration of clone 5
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] )
                        {
                           double sums5i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1); i++ )
                              {
                                 sums5i += Ps[5][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + sums5i )
                                    {
                                       --n[5][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[5] * n[5][i];
                                       Pbtot[5] -= Pb[5][i];
                                       R -= Pb[5][i];
                                       Pb[5][i] = Pb_new;
                                       Pbtot[5] += Pb[5][i];
                                       R += Pb[5][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[5] * n[5][i];
                                       Pstot[5] -= Ps[5][i];
                                       R -= Ps[5][i];
                                       Ps[5][i] = Ps_new;
                                       Pstot[5] += Ps[5][i];
                                       R += Ps[5][i];

                                       // Updating off-host rates of target clone
                                       R -= m[5] * ne[5] + trN[5] * ne[5];
                                       ++ne[5];
                                       R += m[5] * ne[5] + trN[5] * ne[5];
                                       break;
                                    }
                              }
                        } // End emigration of clone 5

                    // Birth of clone 6
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] )
                        {
                           double sumb6i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumb6i += Pb[6][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + sumb6i )
                                    {
                                       ++n[6][i];
                                       ++ntot[i];

                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[6] * n[6][i];
                                       Pbtot[6] -= Pb[6][i];
                                       R -= Pb[6][i];
                                       Pb[6][i] = Pb_new;
                                       Pbtot[6] += Pb[6][i];
                                       R += Pb[6][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[6] * n[6][i];
                                       Pstot[6] -= Ps[6][i];
                                       R -= Ps[6][i];
                                       Ps[6][i] = Ps_new;
                                       Pstot[6] += Ps[6][i];
                                       R += Ps[6][i];
                                       break;
                                    }
                              }
                        } // End birth of clone 6

                     // Symbiont death on host (clone 6)
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] )
                        {
                           double sumd6i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumd6i += Pd[6][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + sumd6i )
                                    {
                                       --n[6][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[6] * n[6][i];
                                       Pbtot[6] -= Pb[6][i];
                                       R -= Pb[6][i];
                                       Pb[6][i] = Pb_new;
                                       Pbtot[6] += Pb[6][i];
                                       R += Pb[6][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[6] * n[6][i];
                                       Pstot[6] -= Ps[6][i];
                                       R -= Ps[6][i];
                                       Ps[6][i] = Ps_new;
                                       Pstot[6] += Ps[6][i];
                                       R += Ps[6][i];
                                       break;
                                    }
                              }
                        } // End death on host (clone 6)

                     // Emigration of clone 6
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] )
                        {
                           double sums6i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1); i++ )
                              {
                                 sums6i += Ps[6][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + sums6i )
                                    {
                                       --n[6][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[6] * n[6][i];
                                       Pbtot[6] -= Pb[6][i];
                                       R -= Pb[6][i];
                                       Pb[6][i] = Pb_new;
                                       Pbtot[6] += Pb[6][i];
                                       R += Pb[6][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[6] * n[6][i];
                                       Pstot[6] -= Ps[6][i];
                                       R -= Ps[6][i];
                                       Ps[6][i] = Ps_new;
                                       Pstot[6] += Ps[6][i];
                                       R += Ps[6][i];

                                       // Updating off-host rates of target clone
                                       R -= m[6] * ne[6] + trN[6] * ne[6];
                                       ++ne[6];
                                       R += m[6] * ne[6] + trN[6] * ne[6];
                                       break;
                                    }
                              }
                        } // End emigration of clone 6

                    // Birth of clone 7
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] )
                        {
                           double sumb7i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumb7i += Pb[7][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + sumb7i )
                                    {
                                       ++n[7][i];
                                       ++ntot[i];

                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[7] * n[7][i];
                                       Pbtot[7] -= Pb[7][i];
                                       R -= Pb[7][i];
                                       Pb[7][i] = Pb_new;
                                       Pbtot[7] += Pb[7][i];
                                       R += Pb[7][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[7] * n[7][i];
                                       Pstot[7] -= Ps[7][i];
                                       R -= Ps[7][i];
                                       Ps[7][i] = Ps_new;
                                       Pstot[7] += Ps[7][i];
                                       R += Ps[7][i];
                                       break;
                                    }
                              }
                        } // End birth of clone 7

                     // Symbiont death on host (clone 7)
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] )
                        {
                           double sumd7i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumd7i += Pd[7][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + sumd7i )
                                    {
                                       --n[7][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[7] * n[7][i];
                                       Pbtot[7] -= Pb[7][i];
                                       R -= Pb[7][i];
                                       Pb[7][i] = Pb_new;
                                       Pbtot[7] += Pb[7][i];
                                       R += Pb[7][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[7] * n[7][i];
                                       Pstot[7] -= Ps[7][i];
                                       R -= Ps[7][i];
                                       Ps[7][i] = Ps_new;
                                       Pstot[7] += Ps[7][i];
                                       R += Ps[7][i];
                                       break;
                                    }
                              }
                        } // End death on host (clone 7)

                     // Emigration of clone 7
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] )
                        {
                           double sums7i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1); i++ )
                              {
                                 sums7i += Ps[7][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + sums7i )
                                    {
                                       --n[7][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[7] * n[7][i];
                                       Pbtot[7] -= Pb[7][i];
                                       R -= Pb[7][i];
                                       Pb[7][i] = Pb_new;
                                       Pbtot[7] += Pb[7][i];
                                       R += Pb[7][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[7] * n[7][i];
                                       Pstot[7] -= Ps[7][i];
                                       R -= Ps[7][i];
                                       Ps[7][i] = Ps_new;
                                       Pstot[7] += Ps[7][i];
                                       R += Ps[7][i];

                                       // Updating off-host rates of target clone
                                       R -= m[7] * ne[7] + trN[7] * ne[7];
                                       ++ne[7];
                                       R += m[7] * ne[7] + trN[7] * ne[7];
                                       break;
                                    }
                              }
                        } // End emigration of clone 7

                    // Birth of clone 8
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] )
                        {
                           double sumb8i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumb8i += Pb[8][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + sumb8i )
                                    {
                                       ++n[8][i];
                                       ++ntot[i];

                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[8] * n[8][i];
                                       Pbtot[8] -= Pb[8][i];
                                       R -= Pb[8][i];
                                       Pb[8][i] = Pb_new;
                                       Pbtot[8] += Pb[8][i];
                                       R += Pb[8][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[8] * n[8][i];
                                       Pstot[8] -= Ps[8][i];
                                       R -= Ps[8][i];
                                       Ps[8][i] = Ps_new;
                                       Pstot[8] += Ps[8][i];
                                       R += Ps[8][i];
                                       break;
                                    }
                              }
                        } // End birth of clone 8

                     // Symbiont death on host (clone 8)
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] )
                        {
                           double sumd8i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumd8i += Pd[8][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + sumd8i )
                                    {
                                       --n[8][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[8] * n[8][i];
                                       Pbtot[8] -= Pb[8][i];
                                       R -= Pb[8][i];
                                       Pb[8][i] = Pb_new;
                                       Pbtot[8] += Pb[8][i];
                                       R += Pb[8][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[8] * n[8][i];
                                       Pstot[8] -= Ps[8][i];
                                       R -= Ps[8][i];
                                       Ps[8][i] = Ps_new;
                                       Pstot[8] += Ps[8][i];
                                       R += Ps[8][i];
                                       break;
                                    }
                              }
                        } // End death on host (clone 8)

                     // Emigration of clone 8
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] )
                        {
                           double sums8i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1); i++ )
                              {
                                 sums8i += Ps[8][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + sums8i )
                                    {
                                       --n[8][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[8] * n[8][i];
                                       Pbtot[8] -= Pb[8][i];
                                       R -= Pb[8][i];
                                       Pb[8][i] = Pb_new;
                                       Pbtot[8] += Pb[8][i];
                                       R += Pb[8][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[8] * n[8][i];
                                       Pstot[8] -= Ps[8][i];
                                       R -= Ps[8][i];
                                       Ps[8][i] = Ps_new;
                                       Pstot[8] += Ps[8][i];
                                       R += Ps[8][i];

                                       // Updating off-host rates of target clone
                                       R -= m[8] * ne[8] + trN[8] * ne[8];
                                       ++ne[8];
                                       R += m[8] * ne[8] + trN[8] * ne[8];
                                       break;
                                    }
                              }
                        } // End emigration of clone 8

                    // Birth of clone 9
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] )
                        {
                           double sumb9i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumb9i += Pb[9][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + sumb9i )
                                    {
                                       ++n[9][i];
                                       ++ntot[i];

                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[9] * n[9][i];
                                       Pbtot[9] -= Pb[9][i];
                                       R -= Pb[9][i];
                                       Pb[9][i] = Pb_new;
                                       Pbtot[9] += Pb[9][i];
                                       R += Pb[9][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[9] * n[9][i];
                                       Pstot[9] -= Ps[9][i];
                                       R -= Ps[9][i];
                                       Ps[9][i] = Ps_new;
                                       Pstot[9] += Ps[9][i];
                                       R += Ps[9][i];
                                       break;
                                    }
                              }
                        } // End birth of clone 9

                     // Symbiont death on host (clone 9)
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] )
                        {
                           double sumd9i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1 ); i++ )
                              {
                                 sumd9i += Pd[9][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + sumd9i )
                                    {
                                       --n[9][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[9] * n[9][i];
                                       Pbtot[9] -= Pb[9][i];
                                       R -= Pb[9][i];
                                       Pb[9][i] = Pb_new;
                                       Pbtot[9] += Pb[9][i];
                                       R += Pb[9][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[9] * n[9][i];
                                       Pstot[9] -= Ps[9][i];
                                       R -= Ps[9][i];
                                       Ps[9][i] = Ps_new;
                                       Pstot[9] += Ps[9][i];
                                       R += Ps[9][i];
                                       break;
                                    }
                              }
                        } // End death on host (clone 9)

                     // Emigration of clone 9
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] )
                        {
                           double sums9i = 0;

                           for ( unsigned int i = 0; i <= ( N - 1); i++ )
                              {
                                 sums9i += Ps[9][i];

                                 if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + sums9i )
                                    {
                                       --n[9][i];
                                       --ntot[i];
                                       // Updating birth rates of target clone and target host
                                       double Pb_new = b[9] * n[9][i];
                                       Pbtot[9] -= Pb[9][i];
                                       R -= Pb[9][i];
                                       Pb[9][i] = Pb_new;
                                       Pbtot[9] += Pb[9][i];
                                       R += Pb[9][i];

                                       // Updating death rates of all the ten clones of the target host
                                       double Pd_new[10] = {0};
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                            Pd_new[j] = ( d[j] + C[j] * ntot[i] ) * n[j][i];
                                            Pdtot[j] -= Pd[j][i];
                                            R -= Pd[j][i];
                                            Pd[j][i] = Pd_new[j];
                                            Pdtot[j] += Pd[j][i];
                                            R += Pd[j][i];
                                          }

                                       // Updating emigration rates of target clone and target host
                                       double Ps_new = s[9] * n[9][i];
                                       Pstot[9] -= Ps[9][i];
                                       R -= Ps[9][i];
                                       Ps[9][i] = Ps_new;
                                       Pstot[9] += Ps[9][i];
                                       R += Ps[9][i];

                                       // Updating off-host rates of target clone
                                       R -= m[9] * ne[9] + trN[9] * ne[9];
                                       ++ne[9];
                                       R += m[9] * ne[9] + trN[9] * ne[9];
                                       break;
                                    }
                              }
                        } // End emigration of clone 9

                     // Host death
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN )
                        {
                           int y = static_cast<int> ( distribution ( generator ) * N );

                           if ( distribution ( generator ) > P ) // If symbionts are released after host death
                              {
                                 for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                    {
                                       R -= m[j] * ne[j] + trN[j] * ne[j];
                                       ne[j] += n[j][y];
                                       R += m[j] * ne[j] + trN[j] * ne[j];
                                    }
                              }

                           for ( int j = 0; j <= ( 10 - 1 ); j++ )
                              {
                                 n[j][y] = 0;
                                 R -= Pb[j][y] + Pd[j][y] + Ps[j][y];
                                 Pbtot[j] -= Pb[j][y];
                                 Pdtot[j] -= Pd[j][y];
                                 Pstot[j] -= Ps[j][y];
                                 Pb[j][y] = 0;
                                 Pd[j][y] = 0;
                                 Ps[j][y] = 0;
                              }
                           ntot[y] = 0;
                        } // End host death

                     // Death of a dispersal symbiont of clone 0
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] )
                        {
                            R -= m[0] * ne[0] + trN[0] * ne[0];
                            --ne[0];
                            R += m[0] * ne[0] + trN[0] * ne[0];
                        } // End of death of dispersal symbiont of clone 0

                     // Transmission of a symbiont of clone 0
                     else  if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] )
                        {
                            R -= m[0] * ne[0] + trN[0] * ne[0];
                            --ne[0];
                            R += m[0] * ne[0] + trN[0] * ne[0];

                           int y = static_cast<int> ( distribution ( generator ) * N );
                           ++n[0][y];
                           ++ntot[y];

                           // Updating birth rates of target clone and target host
                           double Pb_new = b[0] * n[0][y];
                           Pbtot[0] -= Pb[0][y];
                           R -= Pb[0][y];
                           Pb[0][y] = Pb_new;
                           Pbtot[0] += Pb[0][y];
                           R += Pb[0][y];

                           // Updating death rates of all the ten clones of the target host
                           double Pd_new[10] = {0};
                           for ( int j = 0; j <= ( 10 - 1 ); j++ )
                              {
                                 Pd_new[j] = ( d[j] + C[j] * ntot[y] ) * n[j][y];
                                 Pdtot[j] -= Pd[j][y];
                                 R -= Pd[j][y];
                                 Pd[j][y] = Pd_new[j];
                                 Pdtot[j] += Pd[j][y];
                                 R += Pd[j][y];
                              }

                           // Updating emigration rates of target clone and target host
                           double Ps_new = s[0] * n[0][y];
                           Pstot[0] -= Ps[0][y];
                           R -= Ps[0][y];
                           Ps[0][y] = Ps_new;
                           Pstot[0] += Ps[0][y];
                           R += Ps[0][y];
                        } // end of transmission of a symbiont of clone 0

                     // Death of a dispersal symbiont of clone 1
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] )
                        {
                            R -= m[1] * ne[1] + trN[1] * ne[1];
                            --ne[1];
                            R += m[1] * ne[1] + trN[1] * ne[1];
                        } // End of death of dispersal symbiont of clone 1

                     // Transmission of a symbiont of clone 1
                     else  if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] )
                        {
                            R -= m[1] * ne[1] + trN[1] * ne[1];
                            --ne[1];
                            R += m[1] * ne[1] + trN[1] * ne[1];

                           int y = static_cast<int> ( distribution ( generator ) * N );
                           ++n[1][y];
                           ++ntot[y];

                           // Updating birth rates of target clone and target host
                           double Pb_new = b[1] * n[1][y];
                           Pbtot[1] -= Pb[1][y];
                           R -= Pb[1][y];
                           Pb[1][y] = Pb_new;
                           Pbtot[1] += Pb[1][y];
                           R += Pb[1][y];

                           // Updating death rates of all the ten clones of the target host
                           double Pd_new[10] = {0};
                           for ( int j = 0; j <= ( 10 - 1 ); j++ )
                              {
                                 Pd_new[j] = ( d[j] + C[j] * ntot[y] ) * n[j][y];
                                 Pdtot[j] -= Pd[j][y];
                                 R -= Pd[j][y];
                                 Pd[j][y] = Pd_new[j];
                                 Pdtot[j] += Pd[j][y];
                                 R += Pd[j][y];
                              }

                           // Updating emigration rates of target clone and target host
                           double Ps_new = s[1] * n[1][y];
                           Pstot[1] -= Ps[1][y];
                           R -= Ps[1][y];
                           Ps[1][y] = Ps_new;
                           Pstot[1] += Ps[1][y];
                           R += Ps[1][y];
                        } // end of transmission of a symbiont of clone 1

                     // Death of a dispersal symbiont of clone 2
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] )
                        {
                            R -= m[2] * ne[2] + trN[2] * ne[2];
                            --ne[2];
                            R += m[2] * ne[2] + trN[2] * ne[2];
                        } // End of death of dispersal symbiont of clone 2

                     // Transmission of a symbiont of clone 2
                     else  if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] )
                        {
                            R -= m[2] * ne[2] + trN[2] * ne[2];
                            --ne[2];
                            R += m[2] * ne[2] + trN[2] * ne[2];

                           int y = static_cast<int> ( distribution ( generator ) * N );
                           ++n[2][y];
                           ++ntot[y];

                           // Updating birth rates of target clone and target host
                           double Pb_new = b[2] * n[2][y];
                           Pbtot[2] -= Pb[2][y];
                           R -= Pb[2][y];
                           Pb[2][y] = Pb_new;
                           Pbtot[2] += Pb[2][y];
                           R += Pb[2][y];

                           // Updating death rates of all the ten clones of the target host
                           double Pd_new[10] = {0};
                           for ( int j = 0; j <= ( 10 - 1 ); j++ )
                              {
                                 Pd_new[j] = ( d[j] + C[j] * ntot[y] ) * n[j][y];
                                 Pdtot[j] -= Pd[j][y];
                                 R -= Pd[j][y];
                                 Pd[j][y] = Pd_new[j];
                                 Pdtot[j] += Pd[j][y];
                                 R += Pd[j][y];
                              }

                           // Updating emigration rates of target clone and target host
                           double Ps_new = s[2] * n[2][y];
                           Pstot[2] -= Ps[2][y];
                           R -= Ps[2][y];
                           Ps[2][y] = Ps_new;
                           Pstot[2] += Ps[2][y];
                           R += Ps[2][y];
                        } // end of transmission of a symbiont of clone 2

                     // Death of a dispersal symbiont of clone 3
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] )
                        {
                            R -= m[3] * ne[3] + trN[3] * ne[3];
                            --ne[3];
                            R += m[3] * ne[3] + trN[3] * ne[3];
                        } // End of death of dispersal symbiont of clone 3

                     // Transmission of a symbiont of clone 3
                     else  if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] )
                        {
                            R -= m[3] * ne[3] + trN[3] * ne[3];
                            --ne[3];
                            R += m[3] * ne[3] + trN[3] * ne[3];

                           int y = static_cast<int> ( distribution ( generator ) * N );
                           ++n[3][y];
                           ++ntot[y];

                           // Updating birth rates of target clone and target host
                           double Pb_new = b[3] * n[3][y];
                           Pbtot[3] -= Pb[3][y];
                           R -= Pb[3][y];
                           Pb[3][y] = Pb_new;
                           Pbtot[3] += Pb[3][y];
                           R += Pb[3][y];

                           // Updating death rates of all the ten clones of the target host
                           double Pd_new[10] = {0};
                           for ( int j = 0; j <= ( 10 - 1 ); j++ )
                              {
                                 Pd_new[j] = ( d[j] + C[j] * ntot[y] ) * n[j][y];
                                 Pdtot[j] -= Pd[j][y];
                                 R -= Pd[j][y];
                                 Pd[j][y] = Pd_new[j];
                                 Pdtot[j] += Pd[j][y];
                                 R += Pd[j][y];
                              }

                           // Updating emigration rates of target clone and target host
                           double Ps_new = s[3] * n[3][y];
                           Pstot[3] -= Ps[3][y];
                           R -= Ps[3][y];
                           Ps[3][y] = Ps_new;
                           Pstot[3] += Ps[3][y];
                           R += Ps[3][y];
                        } // end of transmission of a symbiont of clone 3

                     // Death of a dispersal symbiont of clone 4
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] + m[4] * ne[4] )
                        {
                            R -= m[4] * ne[4] + trN[4] * ne[4];
                            --ne[4];
                            R += m[4] * ne[4] + trN[4] * ne[4];
                        } // End of death of dispersal symbiont of clone 4

                     // Transmission of a symbiont of clone 4
                     else  if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] + m[4] * ne[4] + trN[4] * ne[4] )
                        {
                            R -= m[4] * ne[4] + trN[4] * ne[4];
                            --ne[4];
                            R += m[4] * ne[4] + trN[4] * ne[4];

                           int y = static_cast<int> ( distribution ( generator ) * N );
                           ++n[4][y];
                           ++ntot[y];

                           // Updating birth rates of target clone and target host
                           double Pb_new = b[4] * n[4][y];
                           Pbtot[4] -= Pb[4][y];
                           R -= Pb[4][y];
                           Pb[4][y] = Pb_new;
                           Pbtot[4] += Pb[4][y];
                           R += Pb[4][y];

                           // Updating death rates of all the ten clones of the target host
                           double Pd_new[10] = {0};
                           for ( int j = 0; j <= ( 10 - 1 ); j++ )
                              {
                                 Pd_new[j] = ( d[j] + C[j] * ntot[y] ) * n[j][y];
                                 Pdtot[j] -= Pd[j][y];
                                 R -= Pd[j][y];
                                 Pd[j][y] = Pd_new[j];
                                 Pdtot[j] += Pd[j][y];
                                 R += Pd[j][y];
                              }

                           // Updating emigration rates of target clone and target host
                           double Ps_new = s[4] * n[4][y];
                           Pstot[4] -= Ps[4][y];
                           R -= Ps[4][y];
                           Ps[4][y] = Ps_new;
                           Pstot[4] += Ps[4][y];
                           R += Ps[4][y];
                        } // end of transmission of a symbiont of clone 4

                     // Death of a dispersal symbiont of clone 5
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] + m[4] * ne[4] + trN[4] * ne[4] + m[5] * ne[5] )
                        {
                            R -= m[5] * ne[5] + trN[5] * ne[5];
                            --ne[5];
                            R += m[5] * ne[5] + trN[5] * ne[5];
                        } // End of death of dispersal symbiont of clone 5

                     // Transmission of a symbiont of clone 5
                     else  if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] + m[4] * ne[4] + trN[4] * ne[4] + m[5] * ne[5] + trN[5] * ne[5] )
                        {
                            R -= m[5] * ne[5] + trN[5] * ne[5];
                            --ne[5];
                            R += m[5] * ne[5] + trN[5] * ne[5];

                           int y = static_cast<int> ( distribution ( generator ) * N );
                           ++n[5][y];
                           ++ntot[y];

                           // Updating birth rates of target clone and target host
                           double Pb_new = b[5] * n[5][y];
                           Pbtot[5] -= Pb[5][y];
                           R -= Pb[5][y];
                           Pb[5][y] = Pb_new;
                           Pbtot[5] += Pb[5][y];
                           R += Pb[5][y];

                           // Updating death rates of all the ten clones of the target host
                           double Pd_new[10] = {0};
                           for ( int j = 0; j <= ( 10 - 1 ); j++ )
                              {
                                 Pd_new[j] = ( d[j] + C[j] * ntot[y] ) * n[j][y];
                                 Pdtot[j] -= Pd[j][y];
                                 R -= Pd[j][y];
                                 Pd[j][y] = Pd_new[j];
                                 Pdtot[j] += Pd[j][y];
                                 R += Pd[j][y];
                              }

                           // Updating emigration rates of target clone and target host
                           double Ps_new = s[5] * n[5][y];
                           Pstot[5] -= Ps[5][y];
                           R -= Ps[5][y];
                           Ps[5][y] = Ps_new;
                           Pstot[5] += Ps[5][y];
                           R += Ps[5][y];
                        } // end of transmission of a symbiont of clone 5

                     // Death of a dispersal symbiont of clone 6
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] + m[4] * ne[4] + trN[4] * ne[4] + m[5] * ne[5] + trN[5] * ne[5] + m[6] * ne[6] )
                        {
                            R -= m[6] * ne[6] + trN[6] * ne[6];
                            --ne[6];
                            R += m[6] * ne[6] + trN[6] * ne[6];
                        } // End of death of dispersal symbiont of clone 6

                     // Transmission of a symbiont of clone 6
                     else  if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] + m[4] * ne[4] + trN[4] * ne[4] + m[5] * ne[5] + trN[5] * ne[5] + m[6] * ne[6] + trN[6] * ne[6] )
                        {
                            R -= m[6] * ne[6] + trN[6] * ne[6];
                            --ne[6];
                            R += m[6] * ne[6] + trN[6] * ne[6];

                           int y = static_cast<int> ( distribution ( generator ) * N );
                           ++n[6][y];
                           ++ntot[y];

                           // Updating birth rates of target clone and target host
                           double Pb_new = b[6] * n[6][y];
                           Pbtot[6] -= Pb[6][y];
                           R -= Pb[6][y];
                           Pb[6][y] = Pb_new;
                           Pbtot[6] += Pb[6][y];
                           R += Pb[6][y];

                           // Updating death rates of all the ten clones of the target host
                           double Pd_new[10] = {0};
                           for ( int j = 0; j <= ( 10 - 1 ); j++ )
                              {
                                 Pd_new[j] = ( d[j] + C[j] * ntot[y] ) * n[j][y];
                                 Pdtot[j] -= Pd[j][y];
                                 R -= Pd[j][y];
                                 Pd[j][y] = Pd_new[j];
                                 Pdtot[j] += Pd[j][y];
                                 R += Pd[j][y];
                              }

                           // Updating emigration rates of target clone and target host
                           double Ps_new = s[6] * n[6][y];
                           Pstot[6] -= Ps[6][y];
                           R -= Ps[6][y];
                           Ps[6][y] = Ps_new;
                           Pstot[6] += Ps[6][y];
                           R += Ps[6][y];
                        } // end of transmission of a symbiont of clone 6

                     // Death of a dispersal symbiont of clone 7
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] + m[4] * ne[4] + trN[4] * ne[4] + m[5] * ne[5] + trN[5] * ne[5] + m[6] * ne[6] + trN[6] * ne[6] + m[7] * ne[7] )
                        {
                            R -= m[7] * ne[7] + trN[7] * ne[7];
                            --ne[7];
                            R += m[7] * ne[7] + trN[7] * ne[7];
                        } // End of death of dispersal symbiont of clone 7

                     // Transmission of a symbiont of clone 7
                     else  if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] + m[4] * ne[4] + trN[4] * ne[4] + m[5] * ne[5] + trN[5] * ne[5] + m[6] * ne[6] + trN[6] * ne[6] + m[7] * ne[7] + trN[7] * ne[7] )
                        {
                            R -= m[7] * ne[7] + trN[7] * ne[7];
                            --ne[7];
                            R += m[7] * ne[7] + trN[7] * ne[7];

                           int y = static_cast<int> ( distribution ( generator ) * N );
                           ++n[7][y];
                           ++ntot[y];

                           // Updating birth rates of target clone and target host
                           double Pb_new = b[7] * n[7][y];
                           Pbtot[7] -= Pb[7][y];
                           R -= Pb[7][y];
                           Pb[7][y] = Pb_new;
                           Pbtot[7] += Pb[7][y];
                           R += Pb[7][y];

                           // Updating death rates of all the ten clones of the target host
                           double Pd_new[10] = {0};
                           for ( int j = 0; j <= ( 10 - 1 ); j++ )
                              {
                                 Pd_new[j] = ( d[j] + C[j] * ntot[y] ) * n[j][y];
                                 Pdtot[j] -= Pd[j][y];
                                 R -= Pd[j][y];
                                 Pd[j][y] = Pd_new[j];
                                 Pdtot[j] += Pd[j][y];
                                 R += Pd[j][y];
                              }

                           // Updating emigration rates of target clone and target host
                           double Ps_new = s[7] * n[7][y];
                           Pstot[7] -= Ps[7][y];
                           R -= Ps[7][y];
                           Ps[7][y] = Ps_new;
                           Pstot[7] += Ps[7][y];
                           R += Ps[7][y];
                        } // end of transmission of a symbiont of clone 7

                     // Death of a dispersal symbiont of clone 8
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] + m[4] * ne[4] + trN[4] * ne[4] + m[5] * ne[5] + trN[5] * ne[5] + m[6] * ne[6] + trN[6] * ne[6] + m[7] * ne[7] + trN[7] * ne[7] + m[8] * ne[8] )
                        {
                            R -= m[8] * ne[8] + trN[8] * ne[8];
                            --ne[8];
                            R += m[8] * ne[8] + trN[8] * ne[8];
                        } // End of death of dispersal symbiont of clone 8

                     // Transmission of a symbiont of clone 8
                     else  if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] + m[4] * ne[4] + trN[4] * ne[4] + m[5] * ne[5] + trN[5] * ne[5] + m[6] * ne[6] + trN[6] * ne[6] + m[7] * ne[7] + trN[7] * ne[7] + m[8] * ne[8] + trN[8] * ne[8] )
                        {
                            R -= m[8] * ne[8] + trN[8] * ne[8];
                            --ne[8];
                            R += m[8] * ne[8] + trN[8] * ne[8];

                           int y = static_cast<int> ( distribution ( generator ) * N );
                           ++n[8][y];
                           ++ntot[y];

                           // Updating birth rates of target clone and target host
                           double Pb_new = b[8] * n[8][y];
                           Pbtot[8] -= Pb[8][y];
                           R -= Pb[8][y];
                           Pb[8][y] = Pb_new;
                           Pbtot[8] += Pb[8][y];
                           R += Pb[8][y];

                           // Updating death rates of all the ten clones of the target host
                           double Pd_new[10] = {0};
                           for ( int j = 0; j <= ( 10 - 1 ); j++ )
                              {
                                 Pd_new[j] = ( d[j] + C[j] * ntot[y] ) * n[j][y];
                                 Pdtot[j] -= Pd[j][y];
                                 R -= Pd[j][y];
                                 Pd[j][y] = Pd_new[j];
                                 Pdtot[j] += Pd[j][y];
                                 R += Pd[j][y];
                              }

                           // Updating emigration rates of target clone and target host
                           double Ps_new = s[8] * n[8][y];
                           Pstot[8] -= Ps[8][y];
                           R -= Ps[8][y];
                           Ps[8][y] = Ps_new;
                           Pstot[8] += Ps[8][y];
                           R += Ps[8][y];
                        } // end of transmission of a symbiont of clone 8

                     // Death of a dispersal symbiont of clone 9
                     else if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] + m[4] * ne[4] + trN[4] * ne[4] + m[5] * ne[5] + trN[5] * ne[5] + m[6] * ne[6] + trN[6] * ne[6] + m[7] * ne[7] + trN[7] * ne[7] + m[8] * ne[8] + trN[8] * ne[8] + m[9] * ne[9] )
                        {
                            R -= m[9] * ne[9] + trN[9] * ne[9];
                            --ne[9];
                            R += m[9] * ne[9] + trN[9] * ne[9];
                        } // End of death of dispersal symbiont of clone 9

                     // Transmission of a symbiont of clone 9
                     else  if ( x < Pbtot[0] + Pdtot[0] + Pstot[0] + Pbtot[1] + Pdtot[1] + Pstot[1] + Pbtot[2] + Pdtot[2] + Pstot[2] + Pbtot[3] + Pdtot[3] + Pstot[3] + Pbtot[4] + Pdtot[4] + Pstot[4] + Pbtot[5] + Pdtot[5] + Pstot[5] + Pbtot[6] + Pdtot[6] + Pstot[6] + Pbtot[7] + Pdtot[7] + Pstot[7] + Pbtot[8] + Pdtot[8] + Pstot[8] + Pbtot[9] + Pdtot[9] + Pstot[9] + MN + m[0] * ne[0] + trN[0] * ne[0] + m[1] * ne[1] + trN[1] * ne[1] + m[2] * ne[2] + trN[2] * ne[2] + m[3] * ne[3] + trN[3] * ne[3] + m[4] * ne[4] + trN[4] * ne[4] + m[5] * ne[5] + trN[5] * ne[5] + m[6] * ne[6] + trN[6] * ne[6] + m[7] * ne[7] + trN[7] * ne[7] + m[8] * ne[8] + trN[8] * ne[8] + m[9] * ne[9] + trN[9] * ne[9] )
                        {
                            R -= m[9] * ne[9] + trN[9] * ne[9];
                            --ne[9];
                            R += m[9] * ne[9] + trN[9] * ne[9];

                           int y = static_cast<int> ( distribution ( generator ) * N );
                           ++n[9][y];
                           ++ntot[y];

                           // Updating birth rates of target clone and target host
                           double Pb_new = b[9] * n[9][y];
                           Pbtot[9] -= Pb[9][y];
                           R -= Pb[9][y];
                           Pb[9][y] = Pb_new;
                           Pbtot[9] += Pb[9][y];
                           R += Pb[9][y];

                           // Updating death rates of all the ten clones of the target host
                           double Pd_new[10] = {0};
                           for ( int j = 0; j <= ( 10 - 1 ); j++ )
                              {
                                 Pd_new[j] = ( d[j] + C[j] * ntot[y] ) * n[j][y];
                                 Pdtot[j] -= Pd[j][y];
                                 R -= Pd[j][y];
                                 Pd[j][y] = Pd_new[j];
                                 Pdtot[j] += Pd[j][y];
                                 R += Pd[j][y];
                              }

                           // Updating emigration rates of target clone and target host
                           double Ps_new = s[9] * n[9][y];
                           Pstot[9] -= Ps[9][y];
                           R -= Ps[9][y];
                           Ps[9][y] = Ps_new;
                           Pstot[9] += Ps[9][y];
                           R += Ps[9][y];
                        } // end of transmission of a symbiont of clone 9

                     // Limitation of output to a frequency of "out_tfreq" days starting from the day "out_tinit"
                     // In addition, the day 1 is always shown by the output
                     if ( time - timeinc < day )
                        {
                           // To finish when symbionts become extinct
                           if ( !full && Nc[0] == 0 && ne[0] < 1 && Nc[1] == 0 && ne[1] < 1 && Nc[2] == 0 && ne[2] < 1 && Nc[3] == 0 && ne[3] < 1 && Nc[4] == 0 && ne[4] < 1 && Nc[5] == 0 && ne[5] < 1 && Nc[6] == 0 && ne[6] < 1 && Nc[7] == 0 && ne[7] < 1 && Nc[8] == 0 && ne[8] < 1 && Nc[9] == 0 && ne[9] < 1 )
                              {
                                  // Obtaining symbiont population parameters (Prev and Ab)
                                  // Prev is the percentage of occupied hosts
                                  // Ab is the mean symbiont population size of occupied hosts

                                  for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                     {
                                  Nc[j] = 0; // Update Nc
                                     }

                                  for ( int i = 0; i <= ( N - 1 ); i++ ) // For each host
                                     {
                                        for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                           {
                                              if ( n[j][i] > 0 )
                                                 ++Nc[j];
                                           }
                                     }
      
                                  float Prev[10] = {0};
      
                                  for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                     {
                                        Prev[j] = static_cast<float> (Nc[j]) / N; // Calculate prevalence for clone 1
                                     }
      
                                  // Calculate mean Abundande in occupied hosts for clone 1
                                  float Ab[10] = {0};

                                  for ( int i = 0; i <= ( N - 1 ); i++ )
                                     {
                                        for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                           if ( n[j][i] > 0 )
                                           {
                                             Ab[j] += n[j][i];
                                           }
                                     }

                                  for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                    {
                                        if ( Nc[j] > 0 )
                                           {
                                              Ab[j] = Ab[j] / Nc[j];
                                           }
                                     }

                                  // Output
                                  cout << step
                                  << ","<< time
                                  << "," << scenarios + Thread
                                  << "," << repetitions
                                  << "," << day;
                                  for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                     {
                                       cout << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev[j]
                                        << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab[j]
                                        << "," << ne[j];
                                     }
                                  cout << endl;

                                 break;
                              }

                           else if ( day % out_tfreq == 0 && day - ( out_tinit - 1 ) > 0 )
                              {
                                 // Obtaining symbiont population parameters (Prev, Ab and VMR)
                                 // Prev is the percentage of occupied hosts
                                 // Ab is the mean symbiont population size of occupied hosts

                                 for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                    {
                                       Nc[j] = 0; // Update Nc
                                    }

                                 for ( int i = 0; i <= ( N - 1 ); i++ ) // For each host
                                    {
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          {
                                             if ( n[j][i] > 0 )
                                                ++Nc[j];
                                          }
                                    }

                                 float Prev[10] = {0};

                                 for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                    {
                                       Prev[j] = static_cast<float> (Nc[j]) / N; // Calculate prevalence for clone 1
                                    }

                                 // Calculate mean Abundande in occupied hosts for clone 1
                                 float Ab[10] = {0};

                                 for ( int i = 0; i <= ( N - 1 ); i++ )
                                    {
                                       for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                          if ( n[j][i] > 0 )
                                          {
                                             Ab[j] += n[j][i];
                                          }
                                    }

                                 for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                    {
                                       if ( Nc[j] > 0 )
                                          {
                                             Ab[j] = Ab[j] / Nc[j];
                                          }
                                    }

                                 // Output
                                 cout << step
                                 << ","<< time
                                 << "," << scenarios + Thread
                                 << "," << repetitions
                                 << "," << day;
                                 for ( int j = 0; j <= ( 10 - 1 ); j++ )
                                    {
                                      cout << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev[j]
                                       << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab[j]
                                       << "," << ne[j];
                                    }
                                 cout << endl;
                              }

                        }

                     ++step;

                  } // End for each step
                    // Ending each simulation

            } // End of repetitions

      } // Ending scenarios to simulate

    return 0;
}
