/************************************************************************
  SYMCOMP
 ************************************************************************/

#include <iostream>
#include <random>
#include <iomanip>
#include <cstdlib> // srand, rand

using namespace std;

int main()
{

   // Initialise variables
   int N, // N is the host population size
       Nc1, // Nc is the number of occupied hosts for clone 1
       Nc2, // Nc is the number of occupied hosts for clone 2
       Nc3, // Nc is the number of occupied hosts for clone 3
       Nc4, // Nc is the number of occupied hosts for clone 4
       Nc5, // Nc is the number of occupied hosts for clone 5
       Nc6, // Nc is the number of occupied hosts for clone 6
       Nc7, // Nc is the number of occupied hosts for clone 7
       Nc8, // Nc is the number of occupied hosts for clone 8
       Nc9, // Nc is the number of occupied hosts for clone 9
       Nc10, // Nc is the number of occupied hosts for clone 10
       T, // T is total number of days of the simulated time
       out_tfreq, //  Frequency of days shown in the output
       out_tinit, // Initial day to start the output
       Rep, // Number of repetitions of each scenario
       N_Scen, // Number of scenarios to simulate
       Thread, // It adjusts the output variable Scen when multiple processors are used
       seed; // Seed used by random generators

   bool    full, // Parameter to determine whether the simulations should be stopped when symbiont become extinct.
                 // True = full simulation; False = simulation stopped when symbionts become extinct.
           Nc_sharing; //  Parameter to determine whether the initial symbionts (n0) either share or use different occupied hosts.


   float   M, // M is the per day and per capita host mortality rate
           P, // P is the probability that a died host was due to predation, leading to death of all of its symbionts

           // Clone 1
           r1, // r is the per day and per capita intrinsic symbiont population growth
           C1, // C is the density dependent parameter of symbiont populations
           s1, // s is the per day and per capita emigration propensity (i.e. dispersal rate)
           n01, // n0 is the initial symbiont population sizes for the initially occupied hosts
           ne1, // ne is the initial number of dispersal symbionts
           m1, // m is the per day and per capita mortality rate of (free-living) dispersal symbionts
           t1, // t is the per day and per capita transmission efficacy of dispersal symbionts

           // Clone 2
           r2,
           C2,
           s2,
           n02,
           ne2,
           m2,
           t2,


           // Clone 3
           r3,
           C3,
           s3,
           n03,
           ne3,
           m3,
           t3,

           // Clone 4
           r4,
           C4,
           s4,
           n04,
           ne4,
           m4,
           t4,

           // Clone 5
           r5,
           C5,
           s5,
           n05,
           ne5,
           m5,
           t5,

           // Clone 6
           r6,
           C6,
           s6,
           n06,
           ne6,
           m6,
           t6,

           // Clone 7
           r7,
           C7,
           s7,
           n07,
           ne7,
           m7,
           t7,

           // Clone 8
           r8,
           C8,
           s8,
           n08,
           ne8,
           m8,
           t8,

           // Clone 9
           r9,
           C9,
           s9,
           n09,
           ne9,
           m9,
           t9,

           // Clone 10
           r10,
           C10,
           s10,
           n010,
           ne10,
           m10,
           t10,

           h; //h is the duration of each running time step of the program

         // Initialization of N_Scen
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
         cin >> Nc1; // Symbiont 1
         cin >> Nc2; // Symbiont 2
         cin >> Nc3; // Symbiont 3
         cin >> Nc4; // Symbiont 4
         cin >> Nc5; // Symbiont 5
         cin >> Nc6; // Symbiont 6
         cin >> Nc7; // Symbiont 7
         cin >> Nc8; // Symbiont 8
         cin >> Nc9; // Symbiont 9
         cin >> Nc10; // Symbiont 10
         cin >> T;
         cin >> out_tfreq;
         cin >> out_tinit;
         cin >> Rep;
         cin >> full;
         cin >> Nc_sharing;
         cin >> M;
         cin >> P;

         // Symbiont 1
         cin >> r1;
         cin >> C1;
         cin >> s1;
         cin >> m1;
         cin >> n01;
         cin >> ne1;
         cin >> t1;

         // Symbiont 2
         cin >> r2;
         cin >> C2;
         cin >> s2;
         cin >> m2;
         cin >> n02;
         cin >> ne2;
         cin >> t2;

         // Symbiont 3
         cin >> r3;
         cin >> C3;
         cin >> s3;
         cin >> m3;
         cin >> n03;
         cin >> ne3;
         cin >> t3;

         // Symbiont 4
         cin >> r4;
         cin >> C4;
         cin >> s4;
         cin >> m4;
         cin >> n04;
         cin >> ne4;
         cin >> t4;

         // Symbiont 5
         cin >> r5;
         cin >> C5;
         cin >> s5;
         cin >> m5;
         cin >> n05;
         cin >> ne5;
         cin >> t5;

         // Symbiont 6
         cin >> r6;
         cin >> C6;
         cin >> s6;
         cin >> m6;
         cin >> n06;
         cin >> ne6;
         cin >> t6;

         // Symbiont 7
         cin >> r7;
         cin >> C7;
         cin >> s7;
         cin >> m7;
         cin >> n07;
         cin >> ne7;
         cin >> t7;

         // Symbiont 8
         cin >> r8;
         cin >> C8;
         cin >> s8;
         cin >> m8;
         cin >> n08;
         cin >> ne8;
         cin >> t8;

         // Symbiont 9
         cin >> r9;
         cin >> C9;
         cin >> s9;
         cin >> m9;
         cin >> n09;
         cin >> ne9;
         cin >> t9;

         // Symbiont 10
         cin >> r10;
         cin >> C10;
         cin >> s10;
         cin >> m10;
         cin >> n010;
         cin >> ne10;
         cin >> t10;

         cin >> h;


         // Variables to reinitialise dynamic parameters after each repetition
         int Nc_init1 = Nc1; // Symbiont 1
         int Nc_init2 = Nc2; // Symbiont 2
         int Nc_init3 = Nc3; // Symbiont 3
         int Nc_init4 = Nc4; // Symbiont 4
         int Nc_init5 = Nc5; // Symbiont 5
         int Nc_init6 = Nc6; // Symbiont 6
         int Nc_init7 = Nc7; // Symbiont 7
         int Nc_init8 = Nc8; // Symbiont 8
         int Nc_init9 = Nc9; // Symbiont 9
         int Nc_init10 = Nc10; // Symbiont 10
         float ne_init1 = ne1; // Symbiont 1
         float ne_init2 = ne2; // Symbiont 2
         float ne_init3 = ne3; // Symbiont 3
         float ne_init4 = ne4; // Symbiont 4
         float ne_init5 = ne5; // Symbiont 5
         float ne_init6 = ne6; // Symbiont 6
         float ne_init7 = ne7; // Symbiont 7
         float ne_init8 = ne8; // Symbiont 8
         float ne_init9 = ne9; // Symbiont 9
         float ne_init10 = ne10; // Symbiont 10

         // Create an array to store symbiont population sizes (ni)
         float n1 [ N ] = {0}; // Symbiont 1
         float n2 [ N ] = {0}; // Symbiont 2
         float n3 [ N ] = {0}; // Symbiont 3
         float n4 [ N ] = {0}; // Symbiont 4
         float n5 [ N ] = {0}; // Symbiont 5
         float n6 [ N ] = {0}; // Symbiont 6
         float n7 [ N ] = {0}; // Symbiont 7
         float n8 [ N ] = {0}; // Symbiont 8
         float n9 [ N ] = {0}; // Symbiont 9
         float n10 [ N ] = {0}; // Symbiont 10

         // Set initial Nc1 to n01
         for ( int counter1 = 1; counter1 <= Nc1; counter1++ )
            {
               n1 [ counter1 - 1 ] = n01;
            }

         // Set initial Nc2-10 to n02-10
         if (Nc_sharing) // initial occupied host are shared
            {
               for ( int counter2 = 1; counter2 <= Nc2; counter2++ )
                  {
                     n2 [ counter2 - 1 ] = n02;
                  }
               for ( int counter3 = 1; counter3 <= Nc3; counter3++ )
                  {
                     n3 [ counter3 - 1 ] = n03;
                  }
               for ( int counter4 = 1; counter4 <= Nc4; counter4++ )
                  {
                     n4 [ counter4 - 1 ] = n04;
                  }
               for ( int counter5 = 1; counter5 <= Nc5; counter5++ )
                  {
                     n5 [ counter5 - 1 ] = n05;
                  }
               for ( int counter6 = 1; counter6 <= Nc6; counter6++ )
                  {
                     n6 [ counter6 - 1 ] = n06;
                  }
               for ( int counter7 = 1; counter7 <= Nc7; counter7++ )
                  {
                     n7 [ counter7 - 1 ] = n07;
                  }
               for ( int counter8 = 1; counter8 <= Nc8; counter8++ )
                  {
                     n8 [ counter8 - 1 ] = n08;
                  }
               for ( int counter9 = 1; counter9 <= Nc9; counter9++ )
                  {
                     n9 [ counter9 - 1 ] = n09;
                  }
               for ( int counter10 = 1; counter10 <= Nc10; counter10++ )
                  {
                     n10 [ counter10 - 1 ] = n010;
                  }
            }

         else // initially occupied host are different for the ten clones
            {
               for ( int counter2 = Nc1 + 1; counter2 <= Nc1 + Nc2 ; counter2++ )
                  {
                     n2 [ counter2 - 1 ] = n02;
                  }
               for ( int counter3 = Nc1 + Nc2 + 1; counter3 <= Nc1 + Nc2 + Nc3 ; counter3++ )
                  {
                     n3 [ counter3 - 1 ] = n03;
                  }
               for ( int counter4 = Nc1 + Nc2 + Nc3 + 1; counter4 <= Nc1 + Nc2 + Nc3 + Nc4 ; counter4++ )
                  {
                     n4 [ counter4 - 1 ] = n04;
                  }
              for ( int counter5 = Nc1 + Nc2 + Nc3 + Nc4 + 1; counter5 <= Nc1 + Nc2 + Nc3 + Nc4 + Nc5 ; counter5++ )
                  {
                     n5 [ counter5 - 1 ] = n05;
                  }
              for ( int counter6 = Nc1 + Nc2 + Nc3 + Nc4 + Nc5 + 1; counter6 <= Nc1 + Nc2 + Nc3 + Nc4 + Nc5 + Nc6 ; counter6++ )
                  {
                     n6 [ counter6 - 1 ] = n06;
                  }
             for ( int counter7 = Nc1 + Nc2 + Nc3 + Nc4 + Nc5 + Nc6 + 1; counter7 <= Nc1 + Nc2 + Nc3 + Nc4 + Nc5 + Nc6 + Nc7 ; counter7++ )
                  {
                     n7 [ counter7 - 1 ] = n07;
                  }
             for ( int counter8 = Nc1 + Nc2 + Nc3 + Nc4 + Nc5 + Nc6 + Nc7 + 1; counter8 <= Nc1 + Nc2 + Nc3 + Nc4 + Nc5 + Nc6 + Nc7 + Nc8 ; counter8++ )
                  {
                     n8 [ counter8 - 1 ] = n08;
                  }
             for ( int counter9 = Nc1+Nc2+Nc3+Nc4+Nc5+Nc6+Nc7+Nc8+1; counter9 <= Nc1+Nc2+Nc3+Nc4+Nc5+Nc6+Nc7+Nc8+Nc9; counter9++ )
                  {
                     n9 [ counter9 - 1 ] = n09;
                  }
             for ( int counter10 = Nc1+Nc2+Nc3+Nc4+Nc5+Nc6+Nc7+Nc8+Nc9+1; counter10 <= Nc1+Nc2+Nc3+Nc4+Nc5+Nc6+Nc7+Nc8+Nc9+Nc10; counter10++ )
                  {
                     n10 [ counter10 - 1 ] = n010;
                  }
            }

         if ( scenarios == 1 ) // Output initial parameter values and headers
            {
               // Print initial parameter values
               cout << "N_Scen = " << N_Scen << endl
               << "Thread = " << Thread << endl
               << "Seed = " << seed << endl
               << "N = " << N << endl
               << "Nc1 = " << Nc1 << endl
               << "Nc2 = " << Nc2 << endl
               << "Nc3 = " << Nc3 << endl
               << "Nc4 = " << Nc4 << endl
               << "Nc5 = " << Nc5 << endl
               << "Nc6 = " << Nc6 << endl
               << "Nc7 = " << Nc7 << endl
               << "Nc8 = " << Nc8 << endl
               << "Nc9 = " << Nc9 << endl
               << "Nc10 = " << Nc10 << endl
               << "T = " << T << endl
               << "out_tfreq = " << out_tfreq << endl
               << "out_tinit = " << out_tinit << endl
               << "Rep = " << Rep << endl
               << "full = " << full << endl
               << "Nc_sharing = " << Nc_sharing << endl
               << "M = " << M << endl
               << "P = " << P << endl

               // Clone 1
               << "r1 = " << r1 << endl
               << "C1 = " << C1 << endl
               << "s1 = " << s1 << endl
               << "m1 = " << m1 << endl
               << "n01 = " << n01 << endl
               << "ne1 = " << ne1 << endl
               << "t1 = " << t1 << endl

               // Clone 2
               << "r2 = " << r2 << endl
               << "C2 = " << C2 << endl
               << "s2 = " << s2 << endl
               << "m2 = " << m2 << endl
               << "n02 = " << n02 << endl
               << "ne2 = " << ne2 << endl
               << "t2 = " << t2 << endl

               // Clone 3
               << "r3 = " << r3 << endl
               << "C3 = " << C3 << endl
               << "s3 = " << s3 << endl
               << "m3 = " << m3 << endl
               << "n03 = " << n03 << endl
               << "ne3 = " << ne3 << endl
               << "t3 = " << t3 << endl

               // Clone 4
               << "r4 = " << r4 << endl
               << "C4 = " << C4 << endl
               << "s4 = " << s4 << endl
               << "m4 = " << m4 << endl
               << "n04 = " << n04 << endl
               << "ne4 = " << ne4 << endl
               << "t4 = " << t4 << endl

               // Clone 5
               << "r5 = " << r5 << endl
               << "C5 = " << C5 << endl
               << "s5 = " << s5 << endl
               << "m5 = " << m5 << endl
               << "n05 = " << n05 << endl
               << "ne5 = " << ne5 << endl
               << "t5 = " << t5 << endl

               // Clone 6
               << "r6 = " << r6 << endl
               << "C6 = " << C6 << endl
               << "s6 = " << s6 << endl
               << "m6 = " << m6 << endl
               << "n06 = " << n06 << endl
               << "ne6 = " << ne6 << endl
               << "t6 = " << t6 << endl

               // Clone 7
               << "r7 = " << r7 << endl
               << "C7 = " << C7 << endl
               << "s7 = " << s7 << endl
               << "m7 = " << m7 << endl
               << "n07 = " << n07 << endl
               << "ne7 = " << ne7 << endl
               << "t7 = " << t7 << endl

               // Clone 8
               << "r8 = " << r8 << endl
               << "C8 = " << C8 << endl
               << "s8 = " << s8 << endl
               << "m8 = " << m8 << endl
               << "n08 = " << n08 << endl
               << "ne8 = " << ne8 << endl
               << "t8 = " << t8 << endl

               // Clone 9
               << "r9 = " << r9 << endl
               << "C9 = " << C9 << endl
               << "s9 = " << s9 << endl
               << "m9 = " << m9 << endl
               << "n09 = " << n09 << endl
               << "ne9 = " << ne9 << endl
               << "t9 = " << t9 << endl

               // Clone 10
               << "r10 = " << r10 << endl
               << "C10 = " << C10 << endl
               << "s10 = " << s10 << endl
               << "m10 = " << m10 << endl
               << "n010 = " << n010 << endl
               << "ne10 = " << ne10 << endl
               << "t10 = " << t10 << endl

               << "h = " << h << endl
               << "\n\n";

               // Print headers for summary results
               cout << "Scen"
               << "," << "Rep"
               << "," << "Day"
               << "," << "Prev1"
               << "," << "Ab1"
               << "," << "ne1"
               << "," << "Prev2"
               << "," << "Ab2"
               << "," << "ne2"
               << "," << "Prev3"
               << "," << "Ab3"
               << "," << "ne3"
               << "," << "Prev4"
               << "," << "Ab4"
               << "," << "ne4"
               << "," << "Prev5"
               << "," << "Ab5"
               << "," << "ne5"
               << "," << "Prev6"
               << "," << "Ab6"
               << "," << "ne6"
               << "," << "Prev7"
               << "," << "Ab7"
               << "," << "ne7"
               << "," << "Prev8"
               << "," << "Ab8"
               << "," << "ne8"
               << "," << "Prev9"
               << "," << "Ab9"
               << "," << "ne9"
               << "," << "Prev10"
               << "," << "Ab10"
               << "," << "ne10"
               << endl;
            }  // Output initial parameter values and headers


         // Repetitions of each scenario
         for ( int repetitions = 1; repetitions <= Rep; repetitions++ )

            {
               // Reinitialisation of dynamic parameters after each repetition
               if (repetitions > 1)
                  {
                     // Reinitialise of Nc and ne after each repetition
                     Nc1 = Nc_init1;
                     Nc2 = Nc_init2;
                     Nc3 = Nc_init3;
                     Nc4 = Nc_init4;
                     Nc5 = Nc_init5;
                     Nc6 = Nc_init6;
                     Nc7 = Nc_init7;
                     Nc8 = Nc_init8;
                     Nc9 = Nc_init9;
                     Nc10 = Nc_init10;
                     ne1 = ne_init1;
                     ne2 = ne_init2;
                     ne3 = ne_init3;
                     ne4 = ne_init4;
                     ne5 = ne_init5;
                     ne6 = ne_init6;
                     ne7 = ne_init7;
                     ne8 = ne_init8;
                     ne9 = ne_init9;
                     ne10 = ne_init10;

                     // Reinitialise array of symbiont population sizes
                     for ( int counter1 = 1; counter1 <= N; counter1++ )
                     {
                       n1 [ counter1 - 1 ] = 0;
                       n2 [ counter1 - 1 ] = 0;
                       n3 [ counter1 - 1 ] = 0;
                       n4 [ counter1 - 1 ] = 0;
                       n5 [ counter1 - 1 ] = 0;
                       n6 [ counter1 - 1 ] = 0;
                       n7 [ counter1 - 1 ] = 0;
                       n8 [ counter1 - 1 ] = 0;
                       n9 [ counter1 - 1 ] = 0;
                       n10 [ counter1 - 1 ] = 0;
                     }

                     // Set initial Nc1 to n01
                     for ( int counter1 = 1; counter1 <= Nc1; counter1++ )
                     n1 [ counter1 - 1 ] = n01;

                    // Set initial Nc2-10 to n02-10
                    if (Nc_sharing) // initially occupied host are shared
                      {
                        for ( int counter2 = 1; counter2 <= Nc2; counter2++ )
                          {
                             n2 [ counter2 - 1 ] = n02;
                          }
                        for ( int counter3 = 1; counter3 <= Nc3; counter3++ )
                          {
                             n3 [ counter3 - 1 ] = n03;
                          }
                        for ( int counter4 = 1; counter4 <= Nc4; counter4++ )
                          {
                             n4 [ counter4 - 1 ] = n04;
                          }
                        for ( int counter5 = 1; counter5 <= Nc5; counter5++ )
                          {
                             n5 [ counter5 - 1 ] = n05;
                          }
                        for ( int counter6 = 1; counter6 <= Nc6; counter6++ )
                          {
                             n6 [ counter6 - 1 ] = n06;
                          }
                        for ( int counter7 = 1; counter7 <= Nc7; counter7++ )
                          {
                             n7 [ counter7 - 1 ] = n07;
                          }
                        for ( int counter8 = 1; counter8 <= Nc8; counter8++ )
                          {
                             n8 [ counter8 - 1 ] = n08;
                          }
                        for ( int counter9 = 1; counter9 <= Nc9; counter9++ )
                          {
                             n9 [ counter9 - 1 ] = n09;
                          }
                        for ( int counter10 = 1; counter10 <= Nc10; counter10++ )
                          {
                             n10 [ counter10 - 1 ] = n010;
                          }
                      }
                    else // initially occupied host are different for both clones
                      {
                        for ( int counter2 = Nc1 + 1; counter2 <= Nc1 + Nc2 ; counter2++ )
                           {
                              n2 [ counter2 - 1 ] = n02;
                           }
                        for ( int counter3 = Nc1 + Nc2 + 1; counter3 <= Nc1 + Nc2 + Nc3 ; counter3++ )
                           {
                              n3 [ counter3 - 1 ] = n03;
                           }
                        for ( int counter4 = Nc1 + Nc2 + Nc3 + 1; counter4 <= Nc1 + Nc2 + Nc3 + Nc4 ; counter4++ )
                           {
                              n4 [ counter4 - 1 ] = n04;
                           }
                        for ( int counter5 = Nc1 + Nc2 + Nc3 + Nc4 + 1; counter5 <= Nc1 + Nc2 + Nc3 + Nc4 + Nc5 ; counter5++ )
                           {
                              n5 [ counter5 - 1 ] = n05;
                           }
                        for ( int counter6 = Nc1 + Nc2 + Nc3 + Nc4 + Nc5 + 1; counter6 <= Nc1 + Nc2 + Nc3 + Nc4 + Nc5 + Nc6 ; counter6++ )
                           {
                              n6 [ counter6 - 1 ] = n06;
                           }
                        for ( int counter7 = Nc1 + Nc2 + Nc3 + Nc4 + Nc5 + Nc6 + 1; counter7 <= Nc1 + Nc2 + Nc3 + Nc4 + Nc5 + Nc6 + Nc7 ; counter7++ )
                           {
                              n7 [ counter7 - 1 ] = n07;
                           }
                        for ( int counter8 = Nc1+Nc2+Nc3+Nc4+Nc5+Nc6+Nc7+1; counter8 <= Nc1+Nc2+Nc3+Nc4+Nc5+Nc6+Nc7+Nc8 ; counter8++ )
                           {
                              n8 [ counter8 - 1 ] = n08;
                           }
                        for ( int counter9 = Nc1+Nc2+Nc3+Nc4+Nc5+Nc6+Nc7+Nc8+1; counter9 <= Nc1+Nc2+Nc3+Nc4+Nc5+Nc6+Nc7+Nc8+Nc9; counter9++ )
                           {
                              n9 [ counter9 - 1 ] = n09;
                           }
                        for ( int counter10 = Nc1+Nc2+Nc3+Nc4+Nc5+Nc6+Nc7+Nc8+Nc9+1; counter10 <= Nc1+Nc2+Nc3+Nc4+Nc5+Nc6+Nc7+Nc8+Nc9+Nc10; counter10++ )
                           {
                              n10 [ counter10 - 1 ] = n010;
                           }
                      }
                  }


               // Starting each simulation
               for ( int step = 1; step <= T / h; step++ ) // For each time step

                  {
                      for ( int counter3 = 0; counter3 <= ( N - 1 ); counter3++ ) // For each host
                         {
                            if ( n1 [ counter3 ] > 0 ||
                                    n2 [ counter3 ] > 0 ||
                                    n3 [ counter3 ] > 0 ||
                                    n4 [ counter3 ] > 0 ||
                                    n5 [ counter3 ] > 0 ||
                                    n6 [ counter3 ] > 0 ||
                                    n7 [ counter3 ] > 0 ||
                                    n8 [ counter3 ] > 0 ||
                                    n9 [ counter3 ] > 0 ||
                                    n10 [ counter3 ] > 0 ) // For occupied hosts
                               {

                                   // Update ni1-10 by adding growth and removing emigration at each step (mechanistically)
                                   float ni1 = n1 [ counter3 ], // Clone 1
                                         ni2 = n2 [ counter3 ], // Clone 2
                                         ni3 = n3 [ counter3 ], // Clone 3
                                         ni4 = n4 [ counter3 ], // Clone 4
                                         ni5 = n5 [ counter3 ], // Clone 5
                                         ni6 = n6 [ counter3 ], // Clone 6
                                         ni7 = n7 [ counter3 ], // Clone 7
                                         ni8 = n8 [ counter3 ], // Clone 8
                                         ni9 = n9 [ counter3 ], // Clone 9
                                         ni10 = n10 [ counter3 ], // Clone 10
                                         ni11,  // Clone 1
                                         ni12,  // Clone 1
                                         ni21,  // Clone 2
                                         ni22,  // Clone 2
                                         ni31,  // Clone 3
                                         ni32,  // Clone 3
                                         ni41,  // Clone 4
                                         ni42,  // Clone 4
                                         ni51,  // Clone 5
                                         ni52,  // Clone 5
                                         ni61,  // Clone 6
                                         ni62,  // Clone 6
                                         ni71,  // Clone 7
                                         ni72,  // Clone 7
                                         ni81,  // Clone 8
                                         ni82,  // Clone 8
                                         ni91,  // Clone 9
                                         ni92,  // Clone 9
                                         ni101,  // Clone 10
                                         ni102;  // Clone 10

                                         // Density-dependent equations showing competitive interaction between both clones
                                         ni11 = ni1 + ( ni1 * ( r1 - C1 * ( ni1 +
                                                                               ni2 +
                                                                               ni3 +
                                                                               ni4 +
                                                                               ni5 +
                                                                               ni6 +
                                                                               ni7 +
                                                                               ni8 +
                                                                               ni9 +
                                                                               ni10  ) ) - s1 * ni1 ) * (h / 2); // Clone 1
                                         ni21 = ni2 + ( ni2 * ( r2 - C2 * ( ni1 +
                                                                               ni2 +
                                                                               ni3 +
                                                                               ni4 +
                                                                               ni5 +
                                                                               ni6 +
                                                                               ni7 +
                                                                               ni8 +
                                                                               ni9 +
                                                                               ni10 ) ) - s2 * ni2 ) * (h / 2); // Clone 2
                                         ni31 = ni3 + ( ni3 * ( r3 - C3 * ( ni1 +
                                                                               ni2 +
                                                                               ni3 +
                                                                               ni4 +
                                                                               ni5 +
                                                                               ni6 +
                                                                               ni7 +
                                                                               ni8 +
                                                                               ni9 +
                                                                               ni10 ) ) - s3 * ni3 ) * (h / 2); // Clone 3
                                         ni41 = ni4 + ( ni4 * ( r4 - C4 * ( ni1 +
                                                                               ni2 +
                                                                               ni3 +
                                                                               ni4 +
                                                                               ni5 +
                                                                               ni6 +
                                                                               ni7 +
                                                                               ni8 +
                                                                               ni9 +
                                                                               ni10 ) ) - s4 * ni4 ) * (h / 2); // Clone 4
                                         ni51 = ni5 + ( ni5 * ( r5 - C5 * ( ni1 +
                                                                               ni2 +
                                                                               ni3 +
                                                                               ni4 +
                                                                               ni5 +
                                                                               ni6 +
                                                                               ni7 +
                                                                               ni8 +
                                                                               ni9 +
                                                                               ni10 ) ) - s5 * ni5 ) * (h / 2); // Clone 5
                                         ni61 = ni6 + ( ni6 * ( r6 - C6 * ( ni1 +
                                                                               ni2 +
                                                                               ni3 +
                                                                               ni4 +
                                                                               ni5 +
                                                                               ni6 +
                                                                               ni7 +
                                                                               ni8 +
                                                                               ni9 +
                                                                               ni10 ) ) - s6 * ni6 ) * (h / 2); // Clone 6
                                         ni71 = ni7 + ( ni7 * ( r7 - C7 * ( ni1 +
                                                                               ni2 +
                                                                               ni3 +
                                                                               ni4 +
                                                                               ni5 +
                                                                               ni6 +
                                                                               ni7 +
                                                                               ni8 +
                                                                               ni9 +
                                                                               ni10 ) ) - s7 * ni7 ) * (h / 2); // Clone 7
                                         ni81 = ni8 + ( ni8 * ( r8 - C8 * ( ni1 +
                                                                               ni2 +
                                                                               ni3 +
                                                                               ni4 +
                                                                               ni5 +
                                                                               ni6 +
                                                                               ni7 +
                                                                               ni8 +
                                                                               ni9 +
                                                                               ni10 ) ) - s8 * ni8 ) * (h / 2); // Clone 8
                                         ni91 = ni9 + ( ni9 * ( r9 - C9 * ( ni1 +
                                                                               ni2 +
                                                                               ni3 +
                                                                               ni4 +
                                                                               ni5 +
                                                                               ni6 +
                                                                               ni7 +
                                                                               ni8 +
                                                                               ni9 +
                                                                               ni10 ) ) - s9 * ni9 ) * (h / 2); // Clone 9
                                         ni101 = ni10 + ( ni10 * ( r10 - C10 * ( ni1 +
                                                                               ni2 +
                                                                               ni3 +
                                                                               ni4 +
                                                                               ni5 +
                                                                               ni6 +
                                                                               ni7 +
                                                                               ni8 +
                                                                               ni9 +
                                                                               ni10 ) ) - s10 * ni10 ) * (h / 2); // Clone 10
                                         ni12 = ni1 + ( ni11 * ( r1 - C1 * ( ni11 +
                                                                                ni21 +
                                                                                ni31 +
                                                                                ni41 +
                                                                                ni51 +
                                                                                ni61 +
                                                                                ni71 +
                                                                                ni81 +
                                                                                ni91 +
                                                                                ni101 ) ) - s1 * ni11 ) * h; // Clone 1
                                         ni22 = ni2 + ( ni21 * ( r2 - C2 * ( ni11 +
                                                                                ni21 +
                                                                                ni31 +
                                                                                ni41 +
                                                                                ni51 +
                                                                                ni61 +
                                                                                ni71 +
                                                                                ni81 +
                                                                                ni91 +
                                                                                ni101 ) ) - s2 * ni21 ) * h; // Clone 2
                                         ni32 = ni3 + ( ni31 * ( r3 - C3 * ( ni11 +
                                                                                ni21 +
                                                                                ni31 +
                                                                                ni41 +
                                                                                ni51 +
                                                                                ni61 +
                                                                                ni71 +
                                                                                ni81 +
                                                                                ni91 +
                                                                                ni101 ) ) - s3 * ni31 ) * h; // Clone 3
                                         ni42 = ni4 + ( ni41 * ( r4 - C4 * ( ni11 +
                                                                                ni21 +
                                                                                ni31 +
                                                                                ni41 +
                                                                                ni51 +
                                                                                ni61 +
                                                                                ni71 +
                                                                                ni81 +
                                                                                ni91 +
                                                                                ni101 ) ) - s4 * ni41 ) * h; // Clone 4
                                         ni52 = ni5 + ( ni51 * ( r5 - C5 * ( ni11 +
                                                                                ni21 +
                                                                                ni31 +
                                                                                ni41 +
                                                                                ni51 +
                                                                                ni61 +
                                                                                ni71 +
                                                                                ni81 +
                                                                                ni91 +
                                                                                ni101 ) ) - s5 * ni51 ) * h; // Clone 5
                                         ni62 = ni6 + ( ni61 * ( r6 - C6 * ( ni11 +
                                                                                ni21 +
                                                                                ni31 +
                                                                                ni41 +
                                                                                ni51 +
                                                                                ni61 +
                                                                                ni71 +
                                                                                ni81 +
                                                                                ni91 +
                                                                                ni101 ) ) - s6 * ni61 ) * h; // Clone 6
                                         ni72 = ni7 + ( ni71 * ( r7 - C7 * ( ni11 +
                                                                                ni21 +
                                                                                ni31 +
                                                                                ni41 +
                                                                                ni51 +
                                                                                ni61 +
                                                                                ni71 +
                                                                                ni81 +
                                                                                ni91 +
                                                                                ni101 ) ) - s7 * ni71 ) * h; // Clone 7
                                         ni82 = ni8 + ( ni81 * ( r8 - C8 * ( ni11 +
                                                                                ni21 +
                                                                                ni31 +
                                                                                ni41 +
                                                                                ni51 +
                                                                                ni61 +
                                                                                ni71 +
                                                                                ni81 +
                                                                                ni91 +
                                                                                ni101 ) ) - s8 * ni81 ) * h; // Clone 8
                                         ni92 = ni9 + ( ni91 * ( r9 - C9 * ( ni11 +
                                                                                ni21 +
                                                                                ni31 +
                                                                                ni41 +
                                                                                ni51 +
                                                                                ni61 +
                                                                                ni71 +
                                                                                ni81 +
                                                                                ni91 +
                                                                                ni101 ) ) - s9 * ni91 ) * h; // Clone 9
                                         ni102 = ni10 + ( ni101 * ( r10 - C10 * ( ni11 +
                                                                                ni21 +
                                                                                ni31 +
                                                                                ni41 +
                                                                                ni51 +
                                                                                ni61 +
                                                                                ni71 +
                                                                                ni81 +
                                                                                ni91 +
                                                                                ni101 ) ) - s10 * ni101 ) * h; // Clone 10
                                         n1 [ counter3 ] = ni12; // Clone1
                                         n2 [ counter3 ] = ni22; // Clone2
                                         n3 [ counter3 ] = ni32; // Clone3
                                         n4 [ counter3 ] = ni42; // Clone4
                                         n5 [ counter3 ] = ni52; // Clone5
                                         n6 [ counter3 ] = ni62; // Clone6
                                         n7 [ counter3 ] = ni72; // Clone7
                                         n8 [ counter3 ] = ni82; // Clone8
                                         n9 [ counter3 ] = ni92; // Clone9
                                         n10 [ counter3 ] = ni102; // Clone10


                                   // Decide if the host dies or survive at his step
                                   std::bernoulli_distribution bernoulli_M(M * h);

                                   if ( bernoulli_M ( generator ) )
                                      // Host die
                                      {
                                         n1 [ counter3 ] = 0;
                                         n2 [ counter3 ] = 0;
                                         n3 [ counter3 ] = 0;
                                         n4 [ counter3 ] = 0;
                                         n5 [ counter3 ] = 0;
                                         n6 [ counter3 ] = 0;
                                         n7 [ counter3 ] = 0;
                                         n8 [ counter3 ] = 0;
                                         n9 [ counter3 ] = 0;
                                         n10 [ counter3 ] = 0;
                                         std::bernoulli_distribution bernoulli_P(P);

                                         if ( bernoulli_P ( generator ) == false)
                                            // Host is not predated
                                            // Update ne by adding released symbionts from host death
                                            {
                                            ne1 += ni11;
                                            ne2 += ni21;
                                            ne3 += ni31;
                                            ne4 += ni41;
                                            ne5 += ni51;
                                            ne6 += ni61;
                                            ne7 += ni71;
                                            ne8 += ni81;
                                            ne9 += ni91;
                                            ne10 += ni101;
                                            }
                                      }
                                   else
                                      // Host survives
                                      // Update ne by adding emigration of the alive host
                                      {
                                         ne1 += s1 * ni11 * h;
                                         ne2 += s2 * ni21 * h;
                                         ne3 += s3 * ni31 * h;
                                         ne4 += s4 * ni41 * h;
                                         ne5 += s5 * ni51 * h;
                                         ne6 += s6 * ni61 * h;
                                         ne7 += s7 * ni71 * h;
                                         ne8 += s8 * ni81 * h;
                                         ne9 += s9 * ni91 * h;
                                         ne10 += s10 * ni101 * h;
                                      }
                               } // End for occupied hosts
                         } // End for each host

                     // Remove random number of dispersal symbionts from ne due to mortality
                     // Random number based on a poisson distribution with mean = m * ne
                        // Clone 1
                     int ne_m1 = static_cast<int> ( ne1 );
                     std::binomial_distribution<int> binomial_m1( ne_m1 , m1 * h );

                     int prand_number1 = binomial_m1 ( generator );

                     if ( prand_number1 < ne1 )
                        {
                           ne1 -= prand_number1;
                        }
                     else
                        {
                            ne1 = 0;
                        }

                        // Clone 2
                     int ne_m2 = static_cast<int> ( ne2 );
                     std::binomial_distribution<int> binomial_m2( ne_m2 , m2 * h );

                     int prand_number2 = binomial_m2 ( generator );

                     if ( prand_number2 < ne2 )
                        {
                           ne2 -= prand_number2;
                        }
                     else
                        {
                            ne2 = 0;
                        }

                        // Clone 3
                     int ne_m3 = static_cast<int> ( ne3 );
                     std::binomial_distribution<int> binomial_m3( ne_m3 , m3 * h );

                     int prand_number3 = binomial_m3 ( generator );

                     if ( prand_number3 < ne3 )
                        {
                           ne3 -= prand_number3;
                        }
                     else
                        {
                            ne3 = 0;
                        }

                        // Clone 4
                     int ne_m4 = static_cast<int> ( ne4 );
                     std::binomial_distribution<int> binomial_m4( ne_m4 , m4 * h );

                     int prand_number4 = binomial_m4 ( generator );

                     if ( prand_number4 < ne4 )
                        {
                           ne4 -= prand_number4;
                        }
                     else
                        {
                            ne4 = 0;
                        }

                        // Clone 5
                     int ne_m5 = static_cast<int> ( ne5 );
                     std::binomial_distribution<int> binomial_m5( ne_m5 , m5 * h );

                     int prand_number5 = binomial_m5 ( generator );

                     if ( prand_number5 < ne5 )
                        {
                           ne5 -= prand_number5;
                        }
                     else
                        {
                            ne5 = 0;
                        }

                        // Clone 6
                     int ne_m6 = static_cast<int> ( ne6 );
                     std::binomial_distribution<int> binomial_m6( ne_m6 , m6 * h );

                     int prand_number6 = binomial_m6 ( generator );

                     if ( prand_number6 < ne6 )
                        {
                           ne6 -= prand_number6;
                        }
                     else
                        {
                            ne6 = 0;
                        }

                        // Clone 7
                     int ne_m7 = static_cast<int> ( ne7 );
                     std::binomial_distribution<int> binomial_m7( ne_m7 , m7 * h );

                     int prand_number7 = binomial_m7 ( generator );

                     if ( prand_number7 < ne7 )
                        {
                           ne7 -= prand_number7;
                        }
                     else
                        {
                            ne7 = 0;
                        }

                        // Clone 8
                     int ne_m8 = static_cast<int> ( ne8 );
                     std::binomial_distribution<int> binomial_m8( ne_m8 , m8 * h );

                     int prand_number8 = binomial_m8 ( generator );

                     if ( prand_number8 < ne8 )
                        {
                           ne8 -= prand_number8;
                        }
                     else
                        {
                            ne8 = 0;
                        }

                        // Clone 9
                     int ne_m9 = static_cast<int> ( ne9 );
                     std::binomial_distribution<int> binomial_m9( ne_m9 , m9 * h );

                     int prand_number9 = binomial_m9 ( generator );

                     if ( prand_number9 < ne9 )
                        {
                           ne9 -= prand_number9;
                        }
                     else
                        {
                            ne9 = 0;
                        }

                        // Clone 10
                     int ne_m10 = static_cast<int> ( ne10 );
                     std::binomial_distribution<int> binomial_m10( ne_m10 , m10 * h );

                     int prand_number10 = binomial_m10 ( generator );

                     if ( prand_number10 < ne10 )
                        {
                           ne10 -= prand_number10;
                        }
                     else
                        {
                            ne10 = 0;
                        }

                     // Transmitted symbionts
                        // Clone 1
                     int ne_t1 = static_cast<int> ( ne1 );
                     std::binomial_distribution<int> binomial_d1( ne_t1, t1 * h * N );
                     int nt1 = static_cast<int> ( binomial_d1 ( generator ) ); // Calculate number of transmitted symbionts

                     if ( nt1 < ne1 ) // Update ne
                        {
                           ne1 -= nt1;
                        }
                     else
                        {
                           ne1 = 0;
                        }

                     for ( int i = 1; i <= nt1; i++ ) // Distribute randomly the transmitted symbionts to hosts
                        {
                           std::uniform_int_distribution<int> rUnif(0,N);
                           ++n1 [ rUnif ( generator ) ];
                        }

                       // Clone 2
                     int ne_t2 = static_cast<int> ( ne2 );
                     std::binomial_distribution<int> binomial_d2( ne_t2, t2 * h * N );
                     int nt2 = static_cast<int> ( binomial_d2 ( generator ) ); // Calculate number of transmitted symbionts

                     if ( nt2 < ne2 ) // Update ne
                        {
                           ne2 -= nt2;
                        }
                     else
                        {
                           ne2 = 0;
                        }

                     for ( int i = 1; i <= nt2; i++ ) // Distribute randomly the transmitted symbionts to hosts
                        {
                           std::uniform_int_distribution<int> rUnif(0,N);
                           ++n2 [ rUnif ( generator ) ];
                        }

                       // Clone 3
                     int ne_t3 = static_cast<int> ( ne3 );
                     std::binomial_distribution<int> binomial_d3( ne_t3, t3 * h * N );
                     int nt3 = static_cast<int> ( binomial_d3 ( generator ) ); // Calculate number of transmitted symbionts

                     if ( nt3 < ne3 ) // Update ne
                        {
                           ne3 -= nt3;
                        }
                     else
                        {
                           ne3 = 0;
                        }

                     for ( int i = 1; i <= nt3; i++ ) // Distribute randomly the transmitted symbionts to hosts
                        {
                           std::uniform_int_distribution<int> rUnif(0,N);
                           ++n3 [ rUnif ( generator ) ];
                        }

                       // Clone 4
                     int ne_t4 = static_cast<int> ( ne4 );
                     std::binomial_distribution<int> binomial_d4( ne_t4, t4 * h * N );
                     int nt4 = static_cast<int> ( binomial_d4 ( generator ) ); // Calculate number of transmitted symbionts

                     if ( nt4 < ne4 ) // Update ne
                        {
                           ne4 -= nt4;
                        }
                     else
                        {
                           ne4 = 0;
                        }

                     for ( int i = 1; i <= nt4; i++ ) // Distribute randomly the transmitted symbionts to hosts
                        {
                           std::uniform_int_distribution<int> rUnif(0,N);
                           ++n4 [ rUnif ( generator ) ];
                        }

                       // Clone 5
                     int ne_t5 = static_cast<int> ( ne5 );
                     std::binomial_distribution<int> binomial_d5( ne_t5, t5 * h * N );
                     int nt5 = static_cast<int> ( binomial_d5 ( generator ) ); // Calculate number of transmitted symbionts

                     if ( nt5 < ne5 ) // Update ne
                        {
                           ne5 -= nt5;
                        }
                     else
                        {
                           ne5 = 0;
                        }

                     for ( int i = 1; i <= nt5; i++ ) // Distribute randomly the transmitted symbionts to hosts
                        {
                           std::uniform_int_distribution<int> rUnif(0,N);
                           ++n5 [ rUnif ( generator ) ];
                        }

                       // Clone 6
                     int ne_t6 = static_cast<int> ( ne6 );
                     std::binomial_distribution<int> binomial_d6( ne_t6, t6 * h * N );
                     int nt6 = static_cast<int> ( binomial_d6 ( generator ) ); // Calculate number of transmitted symbionts

                     if ( nt6 < ne6 ) // Update ne
                        {
                           ne6 -= nt6;
                        }
                     else
                        {
                           ne6 = 0;
                        }

                     for ( int i = 1; i <= nt6; i++ ) // Distribute randomly the transmitted symbionts to hosts
                        {
                           std::uniform_int_distribution<int> rUnif(0,N);
                           ++n6 [ rUnif ( generator ) ];
                        }

                       // Clone 7
                     int ne_t7 = static_cast<int> ( ne7 );
                     std::binomial_distribution<int> binomial_d7( ne_t7, t7 * h * N );
                     int nt7 = static_cast<int> ( binomial_d7 ( generator ) ); // Calculate number of transmitted symbionts

                     if ( nt7 < ne7 ) // Update ne
                        {
                           ne7 -= nt7;
                        }
                     else
                        {
                           ne7 = 0;
                        }

                     for ( int i = 1; i <= nt7; i++ ) // Distribute randomly the transmitted symbionts to hosts
                        {
                           std::uniform_int_distribution<int> rUnif(0,N);
                           ++n7 [ rUnif ( generator ) ];
                        }

                       // Clone 8
                     int ne_t8 = static_cast<int> ( ne8 );
                     std::binomial_distribution<int> binomial_d8( ne_t8, t8 * h * N );
                     int nt8 = static_cast<int> ( binomial_d8 ( generator ) ); // Calculate number of transmitted symbionts

                     if ( nt8 < ne8 ) // Update ne
                        {
                           ne8 -= nt8;
                        }
                     else
                        {
                           ne8 = 0;
                        }

                     for ( int i = 1; i <= nt8; i++ ) // Distribute randomly the transmitted symbionts to hosts
                        {
                           std::uniform_int_distribution<int> rUnif(0,N);
                           ++n8 [ rUnif ( generator ) ];
                        }

                       // Clone 9
                     int ne_t9 = static_cast<int> ( ne9 );
                     std::binomial_distribution<int> binomial_d9( ne_t9, t9 * h * N );
                     int nt9 = static_cast<int> ( binomial_d9 ( generator ) ); // Calculate number of transmitted symbionts

                     if ( nt9 < ne9 ) // Update ne
                        {
                           ne9 -= nt9;
                        }
                     else
                        {
                           ne9 = 0;
                        }

                     for ( int i = 1; i <= nt9; i++ ) // Distribute randomly the transmitted symbionts to hosts
                        {
                           std::uniform_int_distribution<int> rUnif(0,N);
                           ++n9 [ rUnif ( generator ) ];
                        }

                       // Clone 10
                     int ne_t10 = static_cast<int> ( ne10 );
                     std::binomial_distribution<int> binomial_d10( ne_t10, t10 * h * N );
                     int nt10 = static_cast<int> ( binomial_d10 ( generator ) ); // Calculate number of transmitted symbionts

                     if ( nt10 < ne10 ) // Update ne
                        {
                           ne10 -= nt10;
                        }
                     else
                        {
                           ne10 = 0;
                        }

                     for ( int i = 1; i <= nt10; i++ ) // Distribute randomly the transmitted symbionts to hosts
                        {
                           std::uniform_int_distribution<int> rUnif(0,N);
                           ++n10 [ rUnif ( generator ) ];
                        }

                     // Obtaining symbiont population parameters (Prev and Ab)
                     // Prev is the percentage of occupied hosts
                     // Ab is the mean symbiont population size of occupied hosts

                     Nc1 = 0; // Update Nc1
                     Nc2 = 0; // Update Nc2
                     Nc3 = 0; // Update Nc3
                     Nc4 = 0; // Update Nc4
                     Nc5 = 0; // Update Nc5
                     Nc6 = 0; // Update Nc6
                     Nc7 = 0; // Update Nc7
                     Nc8 = 0; // Update Nc8
                     Nc9 = 0; // Update Nc9
                     Nc10 = 0; // Update Nc10

                     for ( int counter4 = 0; counter4 <= ( N - 1 ); counter4++ ) // For each host
                        {
                           // Clone 1
                           if ( n1 [ counter4 ] > 0 )
                              {
                                 ++Nc1;
                              }
                           // Clone 2
                           if ( n2 [ counter4 ] > 0 )
                              {
                                 ++Nc2;
                              }
                           // Clone 3
                           if ( n3 [ counter4 ] > 0 )
                              {
                                 ++Nc3;
                              }
                           // Clone 4
                           if ( n4 [ counter4 ] > 0 )
                              {
                                 ++Nc4;
                              }
                           // Clone 5
                           if ( n5 [ counter4 ] > 0 )
                              {
                                 ++Nc5;
                              }
                           // Clone 6
                           if ( n6 [ counter4 ] > 0 )
                              {
                                 ++Nc6;
                              }
                           // Clone 7
                           if ( n7 [ counter4 ] > 0 )
                              {
                                 ++Nc7;
                              }
                           // Clone 8
                           if ( n8 [ counter4 ] > 0 )
                              {
                                 ++Nc8;
                              }
                           // Clone 9
                           if ( n9 [ counter4 ] > 0 )
                              {
                                 ++Nc9;
                              }
                           // Clone 10
                           if ( n10 [ counter4 ] > 0 )
                              {
                                 ++Nc10;
                              }
                        }

                     float Prev1 = static_cast<float> (Nc1) / N; // Calculate prevalence for clone 1
                     float Prev2 = static_cast<float> (Nc2) / N; // Calculate prevalence for clone 2
                     float Prev3 = static_cast<float> (Nc3) / N; // Calculate prevalence for clone 2
                     float Prev4 = static_cast<float> (Nc4) / N; // Calculate prevalence for clone 2
                     float Prev5 = static_cast<float> (Nc5) / N; // Calculate prevalence for clone 2
                     float Prev6 = static_cast<float> (Nc6) / N; // Calculate prevalence for clone 2
                     float Prev7 = static_cast<float> (Nc7) / N; // Calculate prevalence for clone 2
                     float Prev8 = static_cast<float> (Nc8) / N; // Calculate prevalence for clone 2
                     float Prev9 = static_cast<float> (Nc9) / N; // Calculate prevalence for clone 2
                     float Prev10 = static_cast<float> (Nc10) / N; // Calculate prevalence for clone 2

                     // Calculate mean Abundande in occupied hosts for clone 1
                     float Ab1 = 0;

                     for ( int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
                        {
                           if ( n1 [ counter5 ] > 0 )
                           {
                              Ab1 += n1 [ counter5 ];
                           }
                        }

                     if ( Nc1 > 0 )
                        {
                           Ab1 = Ab1 / Nc1;
                        }

                     // Calculate mean Abundande in occupied hosts for clone 2
                     float Ab2 = 0;

                     for ( int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
                        {
                           if ( n2 [ counter5 ] > 0 )
                           {
                              Ab2 += n2 [ counter5 ];
                           }
                        }

                     if ( Nc2 > 0 )
                        {
                           Ab2 = Ab2 / Nc2;
                        }

                     // Calculate mean Abundande in occupied hosts for clone 3
                     float Ab3 = 0;

                     for ( int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
                        {
                           if ( n3 [ counter5 ] > 0 )
                           {
                              Ab3 += n3 [ counter5 ];
                           }
                        }

                     if ( Nc3 > 0 )
                        {
                           Ab3 = Ab3 / Nc3;
                        }

                     // Calculate mean Abundande in occupied hosts for clone 4
                     float Ab4 = 0;

                     for ( int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
                        {
                           if ( n4 [ counter5 ] > 0 )
                           {
                              Ab4 += n4 [ counter5 ];
                           }
                        }

                     if ( Nc4 > 0 )
                        {
                           Ab4 = Ab4 / Nc4;
                        }

                     // Calculate mean Abundande in occupied hosts for clone 5
                     float Ab5 = 0;

                     for ( int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
                        {
                           if ( n5 [ counter5 ] > 0 )
                           {
                              Ab5 += n5 [ counter5 ];
                           }
                        }

                     if ( Nc5 > 0 )
                        {
                           Ab5 = Ab5 / Nc5;
                        }

                     // Calculate mean Abundande in occupied hosts for clone 6
                     float Ab6 = 0;

                     for ( int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
                        {
                           if ( n6 [ counter5 ] > 0 )
                           {
                              Ab6 += n6 [ counter5 ];
                           }
                        }

                     if ( Nc6 > 0 )
                        {
                           Ab6 = Ab6 / Nc6;
                        }

                     // Calculate mean Abundande in occupied hosts for clone 7
                     float Ab7 = 0;

                     for ( int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
                        {
                           if ( n7 [ counter5 ] > 0 )
                           {
                              Ab7 += n7 [ counter5 ];
                           }
                        }

                     if ( Nc7 > 0 )
                        {
                           Ab7 = Ab7 / Nc7;
                        }

                     // Calculate mean Abundande in occupied hosts for clone 8
                     float Ab8 = 0;

                     for ( int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
                        {
                           if ( n8 [ counter5 ] > 0 )
                           {
                              Ab8 += n8 [ counter5 ];
                           }
                        }

                     if ( Nc8 > 0 )
                        {
                           Ab8 = Ab8 / Nc8;
                        }

                     // Calculate mean Abundande in occupied hosts for clone 9
                     float Ab9 = 0;

                     for ( int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
                        {
                           if ( n9 [ counter5 ] > 0 )
                           {
                              Ab9 += n9 [ counter5 ];
                           }
                        }

                     if ( Nc9 > 0 )
                        {
                           Ab9 = Ab9 / Nc9;
                        }

                     // Calculate mean Abundande in occupied hosts for clone 10
                     float Ab10 = 0;

                     for ( int counter5 = 0; counter5 <= ( N - 1 ); counter5++ )
                        {
                           if ( n10 [ counter5 ] > 0 )
                           {
                              Ab10 += n10 [ counter5 ];
                           }
                        }

                     if ( Nc10 > 0 )
                        {
                           Ab10 = Ab10 / Nc10;
                        }

                     // Limitation of output to a frequency of "out_tfreq" days starting from the day "out_tinit"
                     // In addition, the day 1 is always shown by the output
                     if ( step % static_cast<int>( out_tfreq / h ) == 0 && step - ( ( out_tinit - 1 ) / h ) > 0 ) // Limitation of output
                        {
                           // Output
                           cout << scenarios + Thread
                           << "," << repetitions
                           << "," << static_cast<int> (step * h)
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev1
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab1
                           << "," << ne1
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev2
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab2
                           << "," << ne2
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev3
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab3
                           << "," << ne3
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev4
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab4
                           << "," << ne4
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev5
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab5
                           << "," << ne5
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev6
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab6
                           << "," << ne6
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev7
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab7
                           << "," << ne7
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev8
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab8
                           << "," << ne8
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev9
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab9
                           << "," << ne9
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev10
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab10
                           << "," << ne10 << endl;
                        }

                     // To finish when symbionts are extinct
                     if ( !full && Nc1 == 0 && ne1 < 1 && Nc2 == 0 && ne2 < 1
                                                       && Nc3 == 0 && ne3 < 1
                                                       && Nc4 == 0 && ne4 < 1
                                                       && Nc5 == 0 && ne5 < 1
                                                       && Nc6 == 0 && ne6 < 1
                                                       && Nc7 == 0 && ne7 < 1
                                                       && Nc8 == 0 && ne8 < 1
                                                       && Nc9 == 0 && ne9 < 1
                                                       && Nc10 == 0 && ne10 < 1)

                        {

                           // Output
                           cout << scenarios + Thread
                           << "," << repetitions
                           << "," << static_cast<int> (step * h)
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev1
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab1
                           << "," << ne1
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev2
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab2
                           << "," << ne2
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev3
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab3
                           << "," << ne3
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev4
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab4
                           << "," << ne4
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev5
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab5
                           << "," << ne5
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev6
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab6
                           << "," << ne6
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev7
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab7
                           << "," << ne7
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev8
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab8
                           << "," << ne8
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev9
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab9
                           << "," << ne9
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 3 ) << Prev10
                           << "," << setiosflags ( ios::fixed | ios::showpoint ) << setprecision ( 2 ) << Ab10
                           << "," << ne10 << endl;

                           break;
                        }


                  } // End for each time step
                    // Ending each simulation

            } // End of repetitions

      } // Ending scenarios to simulate

    return 0;
}
