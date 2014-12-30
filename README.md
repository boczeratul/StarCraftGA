## Build & Run
```sh
# Under Linux, Unix, or MacOSX
$ make
$ ./SC2Overmind
```
```sh
# Under Windows
$ ./SC2Overmind.exe
```

## Configuration

### Verification Time (200 ~ 1000 recommanded)
When you start the program, you have to decide how good you want your build order to be.
LARGER NUMBER MAKES BETTER BUILD ORDER!!!
Generally, 300 is enough to find the optimal solution for 7 roach rush.

### Number of requirements 
Then you have to input the number of requirements you need.
For example, if you want 7 roaches, that's "1" requirement.
If you want 7 roaches and 10 zerglings, that's "2" requirements.

### Requirement and Quantity
Specify each requirements. You should enter the requirement ID and the quantity of that requirement. 
The requirement IDs are listed as below:

```sh
# Units   
16: spawn drone  
17: spawn overlord  
18: spawn zergling
19: spawn baneling
20: spawn roach
21: spawn queen
22: spawn hydralisk
23: spawn infestor
24: spawn overseer
25: spawn nydus worm
26: spawn mutalisk
27: spawn corruptor
28: spawn brood lord
29: spawn ultralisk
```
```sh
# Tech
30: upgrade burrow
31: upgrade overlord speed
32: upgrade overlord carriage
33: upgrade melee L1
34: upgrade melee L2
35: upgrade melee L3
36: upgrade range L1
37: upgrade range L2
38: upgrade range L3
39: upgrade defence L1
40: upgrade defence L2
41: upgrade defence L3
42: upgrade zergling speed
43: upgrade zergling attack speed
44: upgrade roach speed
45: upgrade roach burrow movement
46: upgrade baneling speed
47: upgrade hydralisk range
48: upgrade air attack L1
49: upgrade air attack L2
50: upgrade air attack L3
51: upgrade air defence L1
52: upgrade air defence L2
53: upgrade air defence L3
54: upgrade infestor energy
55: upgrade infestor mind control
56: upgrade ultralisk defence
57: upgrade lair tech
58: upgrade hive tech
59: upgrade greater spire tech
```

For the example of 7 roaches, the requirement ID is 20 and the quantity is 7.

After the arguments have been specified, Genetic Algorithm will start.
When it finishe, the best Build Order will be demonstrated.

### Authors  
* Hsuan Lee (MaDBoCs)
* Jen-Hao Chang  
