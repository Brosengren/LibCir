

#process "uNateShip"


// This process has objects with the following auto classes:
class auto_move;
class auto_att_main;

// The following auto classes are not currently used by any objects:
class auto_retro;
class auto_att_fwd;
class auto_att_left;
class auto_att_right;
class auto_att_back;
class auto_att_spike;
class auto_harvest;
class auto_allocate;
class auto_stability;

core_pent_A, 0, 
  {object_burst:auto_att_main, 24},
  {object_burst:auto_att_main, -1809},
  {object_downlink, 256, 
    {component_tri, // component 2
      {object_uplink, 0},
      {object_move:auto_move, -1110},
      {object_move:auto_move, 2048},
    }
  },
  {object_downlink, -256, 
    {component_tri, // component 1
      {object_uplink, 0},
      {object_move:auto_move, -2048},
      {object_move:auto_move, 1110},
    }
  },
  {object_burst:auto_att_main, 1798},

#code




// Process AI modes (these reflect the capabilities of the process)
enum
{
  MODE_IDLE, // process isn't doing anything ongoing
  MODE_MOVE, // process is moving to target_x, target_y
  MODE_GUARD, // process is circling a friendly process
  MODES
};

// Commands that the user may give the process
// (these are fixed and should not be changed, although not all processes accept all commands)
enum
{
  COM_NONE, // no command
  COM_LOCATION, // user has right-clicked somewhere on the display or map
  COM_TARGET, // user has right-clicked on an enemy process
  COM_FRIEND, // user has right-clicked on a friendly process
  COM_DATA_WELL // user has right-clicked on a data well
};

// Targetting information
// Targetting memory allows processes to track targets (enemy or friend)
// The following enums are used as indices in the process' targetting memory
enum
{
  TARGET_PARENT, // a newly built process starts with its builder in address 0
  TARGET_GUARD, // target of guard command
};


// Variable declaration and initialisation
//  (note that declaration and initialisation cannot be combined)
//  (also, variables retain their values between execution cycles)
int core_x, core_y; // location of core
core_x = get_core_x(); // location is updated each cycle
core_y = get_core_y();
int angle; // direction process is pointing
 // angles are in integer degrees from 0 to 8192, with 0 being right,
 // 2048 down, 4096 left and 6144 (or -2048) up.
angle = get_core_angle(); // angle is updated each cycle

int mode; // what is the process doing? (should be one of the MODE enums)

int target_x, target_y; // location of target (to attack, follow etc)
int circle_target; // targetting memory index of target being circled (used by the circle_around_target subroutine)
int circle_rotation; // direction to circle the target in (should be 1024 for clockwise, -1024 for anti-clockwise)
int circle_distance; // distance to maintain from the centre of the circle

int target_component; // target component for an attack command (allows user to
 // target specific components)

int scan_result; // used to hold the results of a scan of nearby processes

int self_destruct_primed; // counter for confirming self-destruct command (ctrl-right-click on self)
int initialised; // set to 1 after initialisation code below run the first time

if (!initialised)
{
   // initialisation code goes here (not all autocoded processes have initialisation code)
  initialised = 1;
  attack_mode(1); // attack objects (if present) will all fire together
  // move the process forward a bit to stop it obstructing the next process to be built
  mode = MODE_MOVE;
}

int verbose; // if 1, process will print various things to the console

if (check_selected_single()) // returns 1 if the user has selected this process (and no other processes)
{
  if (!verbose) printf("\nProcess selected.");
  verbose = 1;
  set_debug_mode(1); // 1 means errors for this process will be printed to the console. Resets to 0 each cycle
}
  else
    verbose = 0;


// Accept commands from user

if (self_destruct_primed > 0)
{
  self_destruct_primed --;
  if (self_destruct_primed == 0
   && verbose)
    printf("\nSelf destruct cancelled.");
}


// What the process does next depends on its current mode
switch(mode)
{
  
  case MODE_IDLE:
    
    
    break;
  
  case MODE_MOVE:
  // stop moving when within 255 pixels of target (can change to higher or lower values if needed)
  //  auto_move.set_power(255);
    
   // auto_move2.set_power(255);\
 //  terminate;
   
   
   int target_found;
   int move_x;
   int move_y;
   //target_found = scan_single(
   //cos(get_core_angle(),400),
   //sin(get_core_angle(),400),
   // 1,
   // 2,
   // 0,
    //12,
   // 0b100100000);
 //  target_found = scan_for_threat(0,0,TARGET_PARENT);
 
  target_found = scan_single(0,0,TARGET_PARENT,0,0,100,0b10);
    printf("%i",target_found);
    auto_att_main.fire(0);
    
    
  if(target_found == 1){
  auto_move.turn_to_target(TARGET_PARENT,0);
//  auto_move.approach_track(TARGET_PARENT, 0,auto_att_main,50);
    }
    else{
      
   // if(distance_from_xy(move_x,move_y) < 300){
    move_x = 800 + random(world_x()-1600);
    move_y = 800 + random(world_y()-1600);
    
    auto_move.move_to(move_x,move_y);
   // }
    }
    
    break;
  
  case MODE_GUARD:
  // Move in circle around friendly target identified by user command
    if (process[TARGET_GUARD].visible()) // returns 1 if target visible. Always returns 1 for a friendly target, if it exists.
    {
      // Now call the circle_around_target subroutine to make this process circle the guarded process
      circle_target = TARGET_GUARD; // circle_target is the process at the centre of the circle (here it's the process being guarded)
      circle_rotation = 1024; // the process will aim 1024 angle units (45 degrees) around the circle, clockwise.
      circle_distance = 700; // the process will try to stay this far from the centre of the circle
 
      break;
    }
    // guard target must have been destroyed. Go back to idle mode and check for new commands.
    clear_command(); // cancels the current command. If there's a queue of commands (e.g. shift-move waypoints)
     //  this moves the queue forward so that check_new_command() will return 1 next cycle.
    mode = MODE_IDLE;
    if (verbose) printf("\nGuard target lost.");
    break;

} // end of mode switch

exit; // stops execution, until the next cycle

// if there are any subroutines (called by gosub statements), they go here
