// EnvTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Modell/rejection_sampling/full_impl.h"
#include "Modell/modell_factory.h"
#include "Data/init_data.h"

#include "Data/data_exporter.h"
#include <thread>

int main()
{
    simulation_params params;

    auto failures = init_failure_datas();
    imperfect_virtualage_likelihood modell;
    modell.set_params_limits({ 0.,10.,0.,3.,0.,1.,0.,1. });
    rejection_sampler<imperfect_virtualage_likelihood> solver;
    solver.setModell(modell);
    solver.setData(failures);
    params.sample_size = 10000000;
    auto p_count = std::thread::hardware_concurrency();
    std::cout << " Ez a program 16 magon ~80s ideig futott. Ez a gep " << p_count << " maggal rendelkezik, es varhatoan ";
    std::cout << static_cast<int>(static_cast<double>(p_count) / 16 * 80) << "s alatt fog lefutni." << std::endl;
    auto begin = std::chrono::steady_clock::now();
    auto result = solver.solve(params);
    auto end = std::chrono::steady_clock::now();
    auto est_time = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    std::cout << "Done in " << est_time << " seconds !" << std::endl;
    std::cout << "Estimeted params are: \nBeta : " << result[0] << "\nEta  : " << result[1] << "\nAr   : " << result[2] << "\nAp   : " << result[3] << std::endl;
    std::cout << "Fileba mentes... Ez eltarthat egy ideig." << std::endl;
    export_data(solver.get_raw_result(), "all_data.csv");
}