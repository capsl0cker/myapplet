flag = 0
def wordBreak(s, dict):
    l = len(dict)
    global flag
    flag = 0

    def find_words(s, i):
        global flag
        print s
        if s.strip() == '':
            flag = 1
            return
        if i >= l:
            flag = 0
            return 
        if s.find(dict[i]) >= 0:
            find_words(s.replace(dict[i], ' ', 1), i)
        if flag == 0:
            find_words(s, i+1)

    find_words(s, 0)
    if flag:
        print "yes"
    else:
        print "no"
 
if __name__ == "__main__":
    s = "aabbaaccaa"
    dict = ["aa", "bbaa", "cc"]
    wordBreak(s, dict)

    

