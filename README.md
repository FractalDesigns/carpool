#carpool Graph

###Description 
---
this c++ class is developed to simply solve the carpooling problem in an optemized way  
The shortest path is calculated using **Bellmann-Ford**' s algorithm.  
The input file has 3 logical units : 
  * `AdjacencyList` 
  * `Demands`
  * `Offers`
  
---
For example the adjacency list' s unit is as follow :
```python
@AdjacencyList:
a->b:3
a->d:1
b->c:2
@end
```

this will construct the following Graph :

![Graph](https://github.com/afritfr/carpool/blob/master/images/example.png)  

`BEWARE : a single line shooldn't hold more than `**one**` edge description ( a->b:3 )` 

the weight on the graph represent the distance

---
The Demands unit looks like this :  
```python
@Demands:
a->c
b->c
@end
```

---

The Offers unit :  
```python
@Offers:
a->c:4
d->b:1
@end
```

the weight here represent the number of available seats per offer

Go ahead and check the example input file [`test.txt`](https://github.com/afritfr/carpool/blob/master/test.txt) and the output file [`result.txt`](https://github.com/afritfr/carpool/blob/master/result.txt) produced by the main()   
