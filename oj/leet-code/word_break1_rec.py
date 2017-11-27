
def wordBreak(s, dict):
    print s
    if len(s) <= 0:
        return True
    flag = False
    for i in range(1, len(s) + 1):
        substr = s[0:i]
        if substr in dict:
            #            if len(substr) == len(s):
            #    return True
            flag = wordBreak(s[i:], dict)
        if flag:
             return True
    return False

s = "aabbaaccaa"
dict = ["aa", "bbaa", "cc"]

if wordBreak(s, dict):
    print 'yes'
else:
    print 'no'
