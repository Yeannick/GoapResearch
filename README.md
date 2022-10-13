# GoapResearch

Goal Oriented Action Planning Research Topic

FrameWork : GamePlayProgramming course Elite FrameWork using SDL

goal : Making a 2D enviroment with AI that uses GOAP decisionMaking structure.

![alt text](https://github.com/Yeannick/GoapResearch/blob/master/Goap.png?raw=true)
----------------------------------------------------------------------------------

GOAP is a decisionmaking structure that uses pathfinding on actions to find the best course of actions to reach a goal.
To do this GOAP needs a couple of things.

- A way to represent the state of the world
- Actions the AI can perform
-  A* Pathfinding algorithm

WORLDSTATE

to represent the state of the world I created a WorldState struct.
It contains a map Variables of string ( key ) and bool (state value) .

the variables are states of the worlds.
e.g  :
```c++
WorldState m_WorldState;
m_WorldState.SetVariable("GunAvailable" , true);
```
a variable added describing if there are guns in the world.

GOALSTATE 

the make a goal state I used the WorldState struct, this way its easier to see if our worldstate meets are goal.
for this reason I added a Priority variable to the worldstate struct to define how much priority a goalstate gets.
multiple goalstates can be made.

-------------------------------------------------------------------------------------
ACTIONS

actions are what our AI can do.

- Pick up a gun 
- Shoot an enemy
- Open a door
- .....

all have a different DO part but all Actions used in GOAP need these things

- PreCondition : Conditions that need to be met by the worldstate for the action to be able to perform.
- Effects : What the action changes to the worldstate after it's been performed.
- Cost : a way to represent the cost of the action ( used in the A* )

in my Implementation the actions needs some extra information

- Target : a position or a GameObject where the action can be performed
- Range : how for away from the Target can we be ( 0 range means no range is needed )

To make Actions using my implementation you create a derived class of base class Action

---------------------------------------------------------------------------------------
PATHFINDING A*

to find a path of actions the AI has to perform we need some sort of pathfinding algorithm.

In Goap we use A* ( A star ).

We give our Algorithm our WorldState , GoalState and All the Actions the AI can perform.

if we set up our worldstate , actions and goal state correctly the algorithm will find a path by checking if the action can perform on the given worldstate 
, checking if its the cheapest one , setting the effects etc.

It is possible that it will not find a path. It highly depends on the state of the world , the preconditions and effects of your actions and the goalstate.
meaning you might want to define more than 1 goalstate so we can check if other goals with less priority can be achieved. 


![alt text](https://github.com/Yeannick/GoapResearch/blob/master/GOAPPath.PNG?raw=true)

-----------------------------------------------------------------------------------------------------------

after the previous parts we already have our action path, but we are not done yet.
We need a way to cycle through the actions.

I used a small FSM ( Finite State Machine ) to solve this problem.

the FSM consist of 3 states :

- Idle : AI plans his path of actions 
- MoveTo : move to target 
- PerformAction : perform the action

by using a FSM I don't need to make RunningTo Actions meaning our pathfinding will be a bit faster.
FSM are also easier to debug than if/else statements in the update.

for the transitions between the states
- Does this Action RequireRange ? No --> PerformAction , Yes --> MoveTo if next statement
- Are we in Range of the target ? No --> MoveTo , Yes --> PerformAction
- Can We Perform Action on worldState ? No --> Idle , Yes --> PerformAction

Sources :
 - https://gamedevelopment.tutsplus.com/tutorials/goal-oriented-action-planning-for-a-smarter-ai--cms-20793
 - https://alumni.media.mit.edu/~jorkin/goap.html
 - https://medium.com/@vedantchaudhari/goal-oriented-action-planning-34035ed40d0b


