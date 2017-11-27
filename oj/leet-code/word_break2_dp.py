
def wordBreak(s, dict):
    res = [False] * (len(s) + 1)
    res[len(s)] = True
    for i in range(len(s) - 1, -1, -1):
        for j in range(i+1, len(s) + 1):
            if res[j] and s[i:j] in dict:
                res[i] = True
                break

    def dfs(str, index):
        if index >= len(s):
            list.append(str.strip())
            print str
            return
        for i in range(index + 1, len(s) + 1):
            if s[index:i] in dict and res[i]:
                str += ' '
                str += s[index:i]
                dfs(str, i)
    str = ''
    list = []
    dfs(str, 0)
    return list

s= "catsanddog"
dict = ["cat", "cats", "and", "sand", "dog"]
wordBreak(s, dict)

             






