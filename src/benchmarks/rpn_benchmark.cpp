//
// Created by to on 22/03/19.
//

//
// Created by to on 20/03/19.
//


#include <string>
#include <vector>
#include <iostream>

#include <Poco/Stopwatch.h>
#include <rpn-processor/rpn_lib.hpp>
#include <rpn-processor/context.hpp>
#include <rpn-processor/script.hpp>
#include <job.hpp>



using std::string;
using std::vector;
using namespace Rpn;

ScriptPtr generateScript(RpnLib *rpnLib)
{
    vector<string> s({
                         "var.get adc0.0",
                         "const.set vsupply",
                         "var.get adc0.1",
                         "const.set isupply",
                         "const.get vsupply",
                         "const.get isupply",
                         "calc_power",
                         "var.set power"
                     });
    Rpn::ScriptPtr script   = rpnLib->compile(s);
    return script;
}

int main(int argc , char const *argv[] )
{
    if ( argc<2 ) {
        fprintf( stderr , "Usage: CMD CONFIG_DIR\n" );
        std::exit(1);
    }

    std::string configPath(argv[1]);

    RpnLib *rpnLib  = new RpnLib();
    rpnLib->init();
    printf( "loading functions..." );
    if ( rpnLib->loadAddons( configPath+"/functions" ) ) {
        printf("SUCCESS\n");
    }
    else {
        printf("ERROR!!!\n");
    }

    long int delta;
    double deltaMs;
    double deltaSecs;
    double rate;


    int NUM_SCRIPTS = 5000;
    printf( "Generating %d scripts..." , NUM_SCRIPTS );
    fflush(stdout);
    std::vector<ScriptPtr> scripts;
    scripts.reserve(NUM_SCRIPTS);
    Poco::Stopwatch stopwatch;
    stopwatch.start();
    for ( int cnt=0 ; cnt<NUM_SCRIPTS ; cnt++ ) {
        scripts.push_back(generateScript(rpnLib));
    }
    delta       = stopwatch.elapsed();
    deltaMs     = delta*0.001;
    deltaSecs   = delta*0.000001;
    rate        = NUM_SCRIPTS/1000.0/deltaSecs;
    printf( " %.1fms ( %.3fk Iter./sec\n",deltaMs,rate);


    int NUM_ITERATIONS = 100000;
    printf( "Evaluating %d scripts..." , NUM_ITERATIONS );
    fflush(stdout);
    stopwatch.restart();
    ContextPtr ctx  = rpnLib->createContext();
    double total=1.0;
    for ( int cnt=0 ; cnt<NUM_ITERATIONS ; cnt++ ) {
        ctx->clear();
        ctx->assignVariable("adc0.0",1.01234);
        ctx->assignVariable("adc0.1",0.99998);
        ScriptPtr script = scripts[cnt%NUM_SCRIPTS];
        script->execute(ctx);
        total *= ctx->recallVariable("power");
    }
    delta       = stopwatch.elapsed();
    deltaMs     = delta*0.001;
    deltaSecs   = delta*0.000001;
    rate        = NUM_ITERATIONS/1000.0/deltaSecs;
    printf( " %.1fms ( %.3fk Iter./sec\n",deltaMs,rate);

}
