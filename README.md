# Simulation of shared resource

### Description

This project is a simulation program for access to a shared resource. The feature is a Sauna (unisex) and the program will be a computer simulation of its use.

Sauna is controlled by a process that receives requests identified by the gender of the client and the estimated duration of occupation. The sauna has a number of places available, which only can be occupied by customers of the same gender. Upon user admission to the sauna, the program controls the time of use, then releasing the place to another user.

Goals Achieved:
* Create multithreaded programs
* Promote intercommunication between processes through named pipes
* Avoid conflicts between competing entities, through synchronization mechanisms

### Technologies/Tools and Languages

```
C
```

### Authors

* Ariana Fernandes
* Bernardo Leite 
* Luís Saraiva


### Explanatory scheme of the sauna

![sauna](https://user-images.githubusercontent.com/22004638/51805330-f213c780-2263-11e9-82a6-faaa366255e0.PNG)
