#include <iostream>

int main() {
    // Constants
    const double MilkyWayStars = 100 * 1e9; // Estimated stars in the Milky Way
    const double AndromedaStars = 100 * 1e9; // Estimated stars in the Andromeda Galaxy
    const double MilkyWayAge = 13.8e9;      // Age of the Milky Way in years

    // Variables for the Milky Way
    double R_star_MW = MilkyWayStars / MilkyWayAge; // Rate of star formation per year in the Milky Way
    double f_p_MW = 0.2;                           // Fraction of Milky Way stars with planets
    double n_e_MW = 1.5;                           // Average number of planets per star with planets
    double f_l_MW = 0.1;                           // Fraction of planets in the Milky Way where life develops
    double f_i_MW = 0.01;                          // Fraction of planets with intelligent life in the Milky Way
    double f_c_MW = 0.5;                           // Fraction of civilizations in the Milky Way with interstellar communication
    double L_MW = 1000;                            // Average civilization lifespan in the Milky Way (in years)

    // Variables for Andromeda (assumptions)
    double R_star_Andromeda = AndromedaStars / MilkyWayAge; // Assumption: Rate of star formation per year in Andromeda
    double f_p_Andromeda = 0.2;                             // Assumption: Fraction of Andromeda stars with planets
    double n_e_Andromeda = 1.5;                             // Assumption: Average number of planets per star with planets
    double f_l_Andromeda = 0.1;                             // Assumption: Fraction of planets in Andromeda where life develops
    double f_i_Andromeda = 0.01;                            // Assumption: Fraction of planets with intelligent life in Andromeda
    double f_c_Andromeda = 0.5;                             // Assumption: Fraction of civilizations in Andromeda with interstellar communication
    double L_Andromeda = 1000;                              // Assumption: Average civilization lifespan in Andromeda (in years)

    // Calculate N for the Milky Way
    double N_MW = R_star_MW * f_p_MW * n_e_MW * f_l_MW * f_i_MW * f_c_MW * L_MW;

    // Calculate N for Andromeda
    double N_Andromeda = R_star_Andromeda * f_p_Andromeda * n_e_Andromeda * f_l_Andromeda * f_i_Andromeda * f_c_Andromeda * L_Andromeda;

    // Display the estimated number of communicative civilizations in both galaxies
    std::cout << "Estimated Number of Communicative Civilizations in the Milky Way: " << N_MW << std::endl;
    std::cout << "Estimated Number of Communicative Civilizations in the Andromeda Galaxy: " << N_Andromeda << std::endl;

    return 0;
}

