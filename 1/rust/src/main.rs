extern crate rand;

use std::time::Instant;

mod timeseries;
use timeseries::*;

fn timeit<F>(f: &mut F)
    where F: FnMut() {
    let t0 = Instant::now();
    f();
    println!("{} nanosec", t0.elapsed().subsec_nanos());
}

fn main() {
    let num_timeseries = 10_000;
    let num_datapoints = 100;

    // step 2
    let mut timeseries = Vec::with_capacity(num_timeseries);
    for _ in 0..num_timeseries {
        timeseries.push(TimeSeries::make_random(num_datapoints))
    }

    let random_timeseries = TimeSeries::make_random(num_datapoints);

    let mut timeseries1 = timeseries.clone();

    for ts in timeseries1.iter_mut() {
        // TODO: ugly that clone is neccecary
        *ts += random_timeseries.clone();
    }

    timeit(&mut || timeseries1.sort());

    // step 3 - Boxed data
    let mut timeseries2 = Vec::with_capacity(num_timeseries);
    for x in timeseries {
        timeseries2.push(Box::new(x));
    }

    for ts in timeseries2.iter_mut() {
        // TODO: ugly that clone is neccecary
        **ts += random_timeseries.clone();
    }

    timeit(&mut || timeseries2.sort());
}
