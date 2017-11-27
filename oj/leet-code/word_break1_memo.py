res = []

def lookup(s, dict, i):
    global res
    if res[i] >= 0:
        return res[i]
    for j in range(0, i):
        if lookup(s, dict, j) and s[j:i] in dict:
            res[i] = 1
            return res[i]
    res[i] = 0
    return res[i] 

def wordBreak(s, dict):
    global res
    res = [-1] * (len(s) + 1)
    res[0] = 1
    return lookup(s, dict, len(s))

s = "a"
dict = [] 

if wordBreak(s, dict):
    print 'yes'
else:
    print 'no'
