import string

def wordBreak(s, dict):
    res = [([0] * (len(s) + 1)) for i in range(len(s))]

    for i in range(0, len(s)):
        for j in range(i+1, len(s) + 1):
            if s[i:j] in dict:
                res[i][j] = 1

    str = []
    """
    按行遍历递归，会超时
    def print_res(l, index):
        if index >= len(s):
            print (string.join(l))
            return
        for i in range(index + 1, len(s) + 1):
            if res[index][i]:
                l.append(s[index:i])
                print_res(l, i)
                l.remove(s[index:i])
    """
    #按列遍历递归，AC
    def print_res(l, index):
        if index <= 0:
            print (string.join(l))
            return
        for i in range(index - 1, -1, -1):
            if res[i][index]:
                l.append(s[i:index])
                print_res(l, i)
                l.remove(s[i:index])

    print_res(str, len(s))



s = "aaaaaaa"
dict = ["aaaa", "aaa"]

wordBreak(s, dict)


