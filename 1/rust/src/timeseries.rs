use std::ops::{Add, AddAssign};
use std::cmp::{Eq, Ord, Ordering, PartialOrd, max};

use rand;
use rand::Rng;

#[derive(Debug, Clone)]
pub struct TimeSeries {
    minimum: i64,
    maximum: i64,
    data: Vec<i64>
}

impl TimeSeries {
    pub fn new(data: &[i64]) -> Self {
        let (min, max) = find_min_max(data);
        TimeSeries {
            minimum: min,
            maximum: max,
            data: data.to_vec(),
        }
    }

    fn update_bounds(&mut self) {
        let (min, max) = find_min_max(&self.data);
        self.minimum = min;
        self.maximum = max;
    }

    fn amplitude(&self) -> i64 {
        self.maximum as i64 - self.minimum as i64
    }

    pub fn make_random(length: usize) -> Self {
        let mut rng = rand::thread_rng();
        let mut data = Vec::with_capacity(length);
        for _ in 0..length {
            data.push(rng.gen());
        }
        TimeSeries::new(&data)
    }
}

impl Eq for TimeSeries{}

impl Ord for TimeSeries {
    fn cmp(&self, other: &Self) -> Ordering {
        self.amplitude().cmp(&other.amplitude())
    }
}

impl PartialEq for TimeSeries {
    fn eq(&self, other: &Self) -> bool {
        self.amplitude() == other.amplitude()
    }
}

impl PartialOrd for TimeSeries {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl AddAssign for TimeSeries {
    fn add_assign(&mut self, other: Self) {
        add_data(&mut self.data, &other.data);
        self.update_bounds();
    }
}
// TODO: i C++ opdaterer vi ikke min og max
impl Add for TimeSeries {
    type Output = Self;
    fn add(self, rhs: Self) -> Self::Output {
        let mut new_data = self.data.clone();
        add_data(&mut new_data, &rhs.data);
        TimeSeries::new(&new_data)
    }
}

fn find_min_max(data: &[i64]) -> (i64, i64) {
    let min = data.into_iter().min().expect("invalid data - no elements");
    let max = data.into_iter().max().expect("invalid data - no elements");
    (*min, *max)
}

fn add_data<T>(left: &mut Vec<T>, right: &Vec<T>)
    where T: Default + Clone + AddAssign {
    // make sure left and right have the same length. Extend the smallest one.
    let longer = max(left.len(), right.len());
    // resize default initializes the new elements (e.g. 0 for int)
    left.resize(longer, T::default());

    let iterator = left.iter_mut()
        .zip(right);

    for (l,r) in iterator {
        *l += r.clone();
    }
}