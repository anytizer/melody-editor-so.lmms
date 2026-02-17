# This is how you define dialects
# could be made directly in code using Macro
# ... somehow
c = key 60
d = key 62
e = key 64
x = rest

# Now you can use them
c d e x e d c x

# Concatenation makes it split the beat
cde

# Supports any characters
ä = key 65
😀 = key 67
🐟 = key 68

ää 😀🐟 😀🐟

# Or words
long = key 75
longer = key 76
lo = key 77

# Matches longest first, no matter what order
# they were defined in
lolongerlonglongerlo

# Can save melodies as variables too
melody = c d ed

# Using it
cc melody d e

# Change of octave and repeat
c' c'' c. c.. c*3

# Works for groups too
melody' melody*2

# Advanced split the beat
((c d) (c d e))

# Extend
c - - -

# Works across split beats
cd -d -c d-c

# Transpose command
c d e transpose 3 c d e transpose 0 c d e

# Want bar lines? Define them as nothing
|=

# They will be ignored
c|d|e

# Oh right, I almost forgot glides
[c d e]

# And you can group thing into a single beat like so
[cd]d

# Its the same as
([(c d)] d)

# Groups are good for repeating
{c d}*2

# Or to make a multi line variable
m = {c d e
     e d e}

m
