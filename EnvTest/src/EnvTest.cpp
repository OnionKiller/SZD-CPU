// EnvTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Modell/rejection_sampling/full_impl.h"
#include "Modell/modell_factory.h"
#include "Data/init_data.h"
#include <fstream>



int main()
{
    simulation_params params;

    auto failures = init_failure_datas();
    imperfect_virtualage_likelihood modell;
    modell.set_params_limits({ 0.,10.,0.,3.,0.,1.,0.,1. });
    rejection_sampler<imperfect_virtualage_likelihood> solver;
    solver.setModell(modell);
    solver.setData(failures);
    std::fstream file("timed_measurement_c_p_data.csv", std::fstream::trunc | std::fstream::out);
    file << "raw_csv"<<std::endl;
    file << "parralel,run time in ms,sample size,beta,eta,ar,ap"<<std::endl;
    for (auto ssize = 10; ssize <= 10000000; ssize *= 10) {
        for (auto I = 100; I-- > 0;) {
            auto begin = std::chrono::steady_clock::now();
            params.sample_size = ssize;
            auto result = solver.solve(params);
            auto end = std::chrono::steady_clock::now();
            auto est_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
            file << false << "," << est_time << "," << ssize << "," << result[0] << "," << result[1] << "," << result[2] << "," << result[3] << std::endl;
            begin = std::chrono::steady_clock::now();
            params.sample_size = ssize;
            result = solver.solve_p(params);
            end = std::chrono::steady_clock::now();
            est_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
            file << true << "," << est_time << "," << ssize << "," << result[0] << "," << result[1] << "," << result[2] << "," << result[3] << std::endl;
        }
    }
    file.close();
    
    
    
    
    
    
    
}