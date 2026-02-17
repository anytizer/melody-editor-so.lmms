### # song strcuture

```
s = key 60
S = key 60
r = key 61
R = key 62
g = key 63
G = key 64
m = key 65
M = key 66
p = key 67
P = key 67
d = key 68
D = key 69
n = key 70
N = key 71
x = rest

music1 = {
    R    G    P    D    n    -    -    -
    D    P    D    -    -    -    -    -
    N    P    D    R    N.   -    -    -
}

music2 = {
    R    G    P    D    n    -    -    -
    N    P    D    R    N.   -    -    -
}

counter = {
    N    P    D    R    N.   -    -    -
}

# now, the song structure
music1*2
music2*2
counter*1
```