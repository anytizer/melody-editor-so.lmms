# @todo Tests pending

### # Defining dialects using macro
#### note name = key number

```
c = key 60
d = key 62
e = key 64
f = key 65
g = key 67
a = key 69
b = key 71
x = rest
```

### # Now you can use them

```
c d e x e d c x
```

### # Concatenation makes it split the beat

```
cde
```

### # Supports any characters

```
ä = key 65
😀 = key 67
🐟 = key 68

ää 😀🐟 😀🐟
```

### # or, words

```
long = key 75
longer = key 76
lo = key 77
```

### # Matches longest first
#### no matter what order they were defined in

```
lolongerlonglongerlo
```

### # Can save melodies as variables too

```
melody = c d ed
```

### # Using saved melody

```
cc melody d e
```

### # Change of octave

```
c' c'' c. c..
```

### # Repeat

```
c*3
```

### # Works for groups too

```
melody' melody*2
```

### # Advanced split the beat

```
((c d) (c d e))
```

### # Extend

```
c - - -
```

### # Works across split beats

```
cd -d -c d-c
```

### # Transpose command

```
c d e transpose 3 c d e transpose 0 c d e
```

### # Parse/Want bar lines?
#### Define them as nothing

```
|=
```

### # Then, they will be ignored

```
c|d|e
```

### # Glides

```
[c d e]
```

### # Group things into a single beat like so

```
[cd]d
```

### # It is same as

```
([(c d)] d)
```

### # Groups are good for repeating

```
{c d}*2
```

### # Or to make a multi line variable

```
m = {c d e
     e d e}

m
```