# SYMDYN
Metapopulation model applied to analyse symbiont dynamics.

# Authors of the model code
Alexandre Mestre, Michael Barfield, James H. Peniston, and Robert D. Holt

This model was used to conduct theoretical research on dispersal evolution in a horizontally transmitted symbiont. The results from this research were presented in a talk at the 2018 ESA Annual Meeting held in New
Orleans, Louisiana:

Mestre A (1), Barfield M (2), Peniston JM (2), Peres-Neto PR (1), Mesquita-Joanes F (3) and Holt RD (2). Tolerance benefit of dispersal when metapopulation conditions become extreme.
(1) Department of Biology, Concordia University, Montreal, QC, Canada
(2) Department of Biology, University of Florida, Gainesville, FL
(3) Cavanilles Institute of Biodiversity and Evolutionary Biology, University of Valencia

## Background/Question/Methods
Dispersal is an essential process in metapopulation dynamics, especially when local disturbance affects the likelihood of local population persistence. Optimal dispersal rates are the ones that maximize metapopulation persistence. In general,
metapopulation models support the presence of a trade-off mechanism in which the optimal dispersal rate increases when intra-patch carrying capacity decreases, and it decreases with increasing mortality during dispersal (often called “cost of
dispersal”). However, under extreme intra- and off-patch conditions, a further harshening of the dispersal environment produces a counterintuitive increase in optimal dispersal rates. This study aims at exploring metapopulation dispersal
dynamics under extreme conditions in detail, by answering the following questions: i) What are the benefits of dispersal when (intra- and off-patch) environmental conditions become extreme? ii) What is the effect of disturbance intensity on the patterns of optimal dispersal strategies under these conditions? iii) What happens to the optimal dispersal rate of individuals that invade empty metapopulations undergoing extreme conditions? We used a spatially-implicit model in which ten clones varying in dispersal rates compete to persist in a metapopulation undergoing local disturbance. The model combined deterministic intra-patch dynamics with logistic, density-dependent growth, and emigration, coupled by stochastic transience among patches mediated by the dispersal environment.

## Results/Conclusions
We confirm that, in a competitive environment with extreme metapopulation conditions, a further increase in both intra- and off-patch harshness leads to higher values of optimal dispersal rate. We also identified an abundance-occupancy trade-off in dispersal between maximizing intra-patch abundance (lower dispersal) and occupancy (higher dispersal), which is conceptually similar to the dominance-tolerance trade-off known for specialist-generalist strategies. In par with this trade-off, we found that dispersal expands the tolerance limits to intra- and off-patch harshness. We also demonstrate that dispersal modifies the optimal environmental conditions in which clones outcompete each other. In addition, disturbance intensity reduces the range of intra- and off-patch environmental conditions that allow for species persistence, intensifying the effects of environmental harshness on optimal dispersal rates. Invasion conditions release higher-dispersal clones with suboptimal dispersal strategies from competitive exclusion, as a result of stochastic loss of stronger competitors at the initial invasion phase. We predict that the tolerance benefits of dispersal will be especially relevant in sink metapopulations where conditions are, by definition, more extreme, and the success of evolutionary rescue for long-term persistence should depend on the details underlying dispersal dynamics.

# Description of the model

The model combined deterministic intra-patch dynamics with logistic, density-dependent growth, and emigration, coupled by stochastic transience among patches mediated by the dispersal environment.

## Aims
Determine whether a symbiont persists in a host population. We focus on the role of host quality and abundance, and the quality of off-host environment. In this model, within-host dynamics and shedding are mechanistic, whereas deaths of hosts and dispersal symbionts, and symbiont colonisation events are stochastic.

## Assumptions
A symbiont population is the number of symbionts within a host.
The symbiont has a direct cycle and can complete the cycle within the same host individual.
Symbiont populations are density-dependent, with a carrying capacity governed by host quality.
The symbiont has continuous generations.
The symbiont has active transmission via an off-host environment with two steps: shedding + transmission.
Shedding rates are a symbiont trait.
Symbionts can be transmitted to both colonized and uncolonized hosts.
Transmission rates depend on off-host symbiont mortality, the capacity of symbionts to move throughout the environment, and the number of hosts free of symbionts.
Survival and displacement efficacy during transmission are governed by the quality of the off-host environment.
Host population size remains constant over time.
Host mortality is constant and independent of symbiont dynamics (i.e. commensal).
When a host dies, symbionts can either be released to take part in the dispersal pool or die because the host was fed by a predator.
Within-host dynamics and shedding will be mechanistic.
Second-order Runge-Kutta algorithm will be used for calculations of population increase each time step.
Host mortality will be stochastic.
Colonisation will be stochastic.

## Variables
N is the host population size.
Nc is the number of colonized hosts.
n0 is the initial symbiont population size for initially colonized hosts
ne is the number of dispersal symbionts (i.e. living out of the hosts).
M is the per day and per capita host mortality rate.
P is the probability that a deed host was due to predation.
r is the basic symbiont population growth (within a host).
C is the density-dependent parameter of symbiont populations.
s is the per day and per capita shedding rate.
m is the per day and per capita mortality rate of dispersal symbionts.
t is the per day and per capita transmission efficacy of dispersal symbionts.
ni is the symbiont population size within the host “i”
Prev is the percentage of colonized hosts.
Ab is the mean symbiont population size of colonized hosts.
VMR is the variance to mean ratio of symbiont population sizes of colonized hosts.
T is the total number of days of the experiment.
h is the duration of each running time step of the program.
out_tfreq is the scaling parameter controlling the frequency of days shown in the output
out_tinit is the shifting parameter controlling the initial day for the output
Rep is the number of repetitions of each scenario
N_Scen is the number of scenarios to be simulated

## Equations
Eq. 1. Estimation of ni increase at each time step using Runge-Kutta algorithm method 

ni+1/2 = ni + [ ni ( r - C * ni ) - s * ni ] * ½ * h
ni+1 = ni + [ ni+1/2 ( r - C * ni+1/2 ) - s * ni+1/2 ] * h

Eq. 2. Calculation of prevalence (Prev) at each time step

Prev = Nc / N

Eq. 3. Calculation of mean abundance of colonized hosts (Ab) at each time step

Ab = sum (symbiont population sizes) / Nc

## Pseudo-code
(top-down stepwise refinement)

### Problem statement
Determine whether a symbiont persists in a host population, depending on host quality and abundance, and the quality of the off-host environment.

### First refinement
Initialize variables
Simulate the symbiont dynamics along the experimental time
Print temporal variation of symbiont population parameters

### Second refinement
Initialize N to value  // N is the host population size (that remains constant in all the experiments)
Initialize Nc to value // Nc is the number of colonized hosts
Initialize n0 to value // n0 is the initial symbiont population size for initially colonized hosts
Initialize ne to value // ne is the number of dispersal symbionts
Initialize M to value // M is the per day and per capita host mortality rate
Initialize P to value // P is a per day and per capita predation rate on the host
Initialize r to value // r is the basic symbiont population growth
Initialize C to value // C is the density-dependent parameter of symbiont populations
Initialize s to value // s is the per day and per capita shedding rate
Initialize m to value // m is the per day and per capita mortality rate of dispersal symbionts
Initialize t to value // t is the per day and per capita movement efficacy of dispersal symbionts
Initialize T to value // T is the total number of days of the experiment
Initialize h to value // h is the  running time step of the program

Set initial conditions of within-host populations

For each time step (until experimental time finishes or prevalence gets 0)
   Calculate (deterministically) initial symbiont population sizes at the given step
   Host deaths and births
   Calculate the number of dispersal symbionts
   Calculate the number of symbionts that colonize a host
   Assign transmitted symbionts to new hosts
   Update symbiont population sizes
   Print symbiont population parameters at each step

### Third refinement
Initialize N to value
Initialize Nc to value
Inizialize n0 to value
Initialize ne to value
Initialize M to value
Initialize P to value
Initialize r to value
Initialize C to value
Initialize s to value
Initialize m to value
Initialize t to value
Initialize T to value
Initialize h to value

Set the value of ni of each of the N hosts to 0.
Select initially colonized hosts and update their ni to n0.

For each time step (until the number of steps gets T)

   For each host (from 1 to N)
      If the host is colonized
         Calculate the ni+1 at the given step with the Runge-Kutta algorithm method (Eq. 1)
         Update ni to ni+1
         // Decide if the host dies
        Select a random number based on M and Bernoulli distribution to see if the death event occurs.
        If the death event occurs
           Set its ni to 0 (i.e. it is replaced by a newborn host)
           Select a rand. numb. based on P and Bernoulli dist. to see whether the host was predated.
              If it was not predated
                 Update ne by adding the released symbionts from host death
        Else (i.e. host survive)
           Update ne by adding shedding of the alive host

   // Calculation of the number of dispersal symbionts that die during the step
   Select a random number from a binomial distribution (ne , m * h)
   If the random number is lower than ne
      Update ne to ne minus the random number (i.e. died symbionts)
   Else (i.e. random number equal or higher than ne)
      Update ne to 0

   // Symbiont transmissions
   Calculate stochastically the number of transmitted symbionts from a binomial (ne, t * h * N)
   Assign each of the transmitted symbionts to new hosts randomly
   Update values of symbiont ni accordingly

   // Results
   Update Nc
   Calculate Prev at this step (Eq. 2)
   Calculate Ab at this step (Eq. 3)
   Print summary of results at each step:
      - Time step
      - Prev
      - Ab
      - VMR
      - ne
