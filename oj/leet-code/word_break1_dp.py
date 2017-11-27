
def wordBreak(s, dict):
    res = [False] * (len(s) + 1)
    res[0] = True
    for i in range(1, len(s) + 1):
        for j in range(0, i):
            if res[j] and s[j:i] in dict:
                res[i] = True
                break
    return res[len(s)]

s="aabbaaccaa"
dict = ['aa', 'bbaa', 'cc']

if wordBreak(s, dict):
    print 'yes'
else:
    print 'no'

