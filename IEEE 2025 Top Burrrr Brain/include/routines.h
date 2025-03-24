#include "ferris_wheel_n_sorting.h"
#include "buckets_n_mast.h"



////////////////////////////////// Bucket Pickup and Dropoff Routines ////////////////////////////////////

// At right bucket ready for pickup. Picks up right bucket and lifts all the way up.
// ASSUMES STEPPERS ARE ALL THE WAY UP
void right_bucket_pickup(){     
    lower_buckets_top_to_bottom();      // Lower buckets from very top position to very bottom position
    pinch_right_bucket();
    lift_buckets_bottom_to_top();       // Lift buckets from very bottom position to very top position
    Serial.println("Right bucket picked up");
}

// At left bucket ready for pickup. Picks up left bucket and lifts to travel height.
// ASSUMES THE STEPPERS ARE ALL THE WAY UP
void left_bucket_pickup(){
    lower_buckets_top_to_bottom();      // Lower buckets from very top position to very bottom position
    pinch_left_bucket();
    lift_buckets_to_travel();           // Lift buckets from very bottom to travel height
    Serial.println("Left bucket picked up");
}

// ASSUMES THE STEPPERS ARE AT TRAVEL HEIGHT
void right_bucket_dropoff(){
    release_right_bucket();             // Drops bucket from travel height
    lift_buckets_travel_to_top();       // Lift buckets from travel height to very top position
}

// CURRENTLY ASSUMING THE LEFT BUCKET IS ALL THE WAY UP
void left_bucket_dropoff(){
    lower_buckets_top_to_travel();      // Lower buckets from very top position to travel height
    release_left_bucket();              // Drops bucket from travel height
    lift_buckets_travel_to_top();       // Lift buckets from travel height to very top position
}

