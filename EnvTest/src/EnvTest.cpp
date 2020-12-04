// EnvTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Modell/rejection_sampling/impl.h"
#include "Modell/modell_factory.h"
#include "Data/init_data.h"



int main()
{
    simulation_params params;

    auto failures = init_failure_datas();
    imperfect_virtualage_likelihood modell;
    modell.set_params_limits({ 0.,10.,0.,3.,0.,1.,0.,1. });
    rejection_sampler solver;
    solver.setModell(modell);
    solver.setData(failures);
    params.sample_size = 1000000;
    auto result = solver.solve(params);
    std::cout << "Done!\n";
    std::cout << "Estimeted params are: \nBeta : " << result[0] << "\nEta  : " << result[1] << "\nAr   : " << result[2] << "\nAp   : " << result[3] << std::endl;
}