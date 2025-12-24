//
// Created by magofrays on 12/13/25.
//

#include "simulation/BoostSimulationBuilder.h"
#include "simulation/MessengerSimulationAdapter.h"
#include "simulation/MessengerSimulationBuilder.h"
#include "simulation/Simulation.h"
#include "utils/LSM.h"
#include "types/StateVector.h"

int main(int argc, char* argv[])
{
    Simulation *simulation = BoostSimulationBuilder().buildSimulation();
    // simulation->run(true);
    // return 0;

    SpiceDouble start_time = 1993302913.5 - 251;
    SpiceDouble end_time = 1993303873.5 - 251;


    MessengerSimulationAdapter adapter(simulation);
    ExtendedBodyState start_body_state(
    Vec3d(-3.018808873610819E-01,  1.391836383670322E-01,  1.057009733205437E-01),
    Vec3d(-1.976064346474170E-02, -2.167599514783350E-02, -8.323094213103985E-03),
    start_time / day
    );


    //поместить фал с данными мессенджера по пути forward-task/msgr_120501_130430_recon_gsfc_1.bsp



    StateVector state = StateVector();
    StateVector out_state;
    for (int i = 0; i < 1; i++) {
        std::cout << "cur_pos: " << std::setprecision(17) << start_body_state.position << std::endl;
        std::cout << "cur_vel: " << std::setprecision(17) << start_body_state.velocity << std::endl;
        auto result = adapter.get_messenger_between_km(start_time/day, end_time/day, 10/day, start_body_state);
        state.add_state(BodyState(start_body_state.position, start_body_state.velocity, start_body_state.time));
        LSM* lsm = new LSM(state, *result, "for_c_2013_last.txt", start_time/day, end_time/day);
        out_state = lsm->do_LSM(1);
        std::cout << "cur_pos: " << std::setprecision(17) << out_state.state.position << std::endl;
        std::cout << "new_pos: " << std::setprecision(17) << start_body_state.position << std::endl;
        start_body_state.position = out_state.state.position;
        start_body_state.velocity = out_state.state.velocity;
        std::cout << "----------------------------------------------------------------------------------------------\n";
        delete lsm;
    }

    /*(-0.11327381821320991, -0.40656486930886337, -0.20537705477876680)
    (0.02028347765177638, -0.00341450216897086, -0.00527430528069032)

    (-0.11327343862093999, -0.40656638033640327, -0.20537424549056479)
    (0.02028347765177638, -0.00341450216897086, -0.00527430528069032)

    (-0.11327343862093999, -0.40656638033640327, -0.20537424549056479)
    (0.02028347765177638, -0.00341450216897086, -0.00527430528069032)*/

    delete simulation;
    return 0;
}


/*
SpiceDouble start_time = 2011020795;
    SpiceDouble end_time = 2011023800;
    MessengerSimulationAdapter adapter(simulation);
    ExtendedBodyState start_body_state(
    Vec3d(-1.517223966758399E-01, -3.970536370792114E-01, -1.961882991612178E-01),
    Vec3d(2.009642721040850E-02, -6.547422135645379E-03, -4.035817111972329E-03),
    start_time / day
    );
 */

//-0.11327381821320991, -0.40656486930886337, -0.20537705477876680
//-0.11327381668142248, -0.40656487040112670, -0.20537704700866269



//1993302918 500 11 500 137079 5667936 25 -589472 -124111175 0 77000 1
//1993302918,2013-03-01 15:15:18,-589472.124111175,-589472,-124111175,25,0,2,2299809660.0,137079,5667936,11,0,2,0.0


//(-0.30188088736108187, 0.13918363836703221, 0.10570097332054371)
//(-0.30188803760780084, 0.13921627138813316, 0.10563754269726123)