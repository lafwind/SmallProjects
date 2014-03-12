import urllib
import urllib2
import re

Matric = {}
def downURL(url, filename):
    """docstring for downURL"""
    try:
        fp = urllib2.urlopen(url, timeout = 19)
    except:
        print 'download exception!'
        return 0
    op = open('/home/lafwind/Source_Codes/Python/My_Program/Spider/Pages/'+filename, 'wb')
    a = fp.read()
    op.write(a)
    fp.close()
    op.close()
    return 1

def getURL(url):
    """docstring for getURL"""
    try:
        fp = urllib2.urlopen(url, timeout = 99)
    except:
        print 'get URL exception'
        return []

    pattern = re.compile("http://www.nju.edu.cn/[^:\|\>\'\"\s]*|http://www.nju.edu.cn:80/[^\>\'\"\s]*|http://[^\?\|\:\s]+.nju.edu.cn") #[^\'\"\s]*/   ((?!.jpg).)*$(?!\.jpg )^(?!\.swf)
    wrong_pattern = re.compile("http://www.nju.edu.cn:80/[^:\|\>\'\"\s]+.jpg|http://www.nju.edu.cn:80/[^:\|\>\'\"\s]+.gif")
    a = fp.read()
    urls = pattern.findall(a)
    wrong_urls = wrong_pattern.findall(a)
    #for i in urls:
    #    wrong_urls = re.findall('.*jpg$', i)
    for k in urls:
        if k in wrong_urls:
            urls.remove(k)
    fp.close()
    return urls

def out(dict, filename, splitby, addstr):
    """docstring for out"""
    temp = open(filename, 'wb')
    strdict = str(dict)
    temp.write(strdict)
    temp.close()
    f = open(filename, 'r')
    all = f.read()
    line = all.split(splitby)
    temp_str = ""
    for i in line:
        temp_str += i + addstr
    result = open(filename, 'wb')
    result.write(temp_str)
    result.close()

def creat_Matric(M):
    """docstring for make_Matric"""
    str = ""
    str1 = ""
    str2 = ""
    m = open('Matric.txt', 'wb')
    for h in M:
        str1 = str1 + h + "\r\n"
        for k in M:
            if k in M[h]:
                str2 = str2 + '1' + '   '
            else:
                str2 = str2 + '0' + '   '
        str2 = str2 + '\r\n'

    str = str1 + str2
    m.write(str.encode('utf-8'))
    m.close()

def spider(startURL):
    """docstring for spider"""
    urls = []
    visited = {}
    bevisited = {}
    link_url = {}
    urls.append(startURL)
    link_url[startURL] = 0
    bevisited[startURL] = []
    i = 0
    while True:
        if len(urls) > 0:
            url = urls.pop(0)
            a = downURL(url,str(i)+'.html')
            if a == 1:
                print url
                i = i + 1
                urllist = getURL(url)
                visited[url] = urllist
                for li in urllist:
                    if li in link_url:
                        link_url[li] = link_url[li] + 1
                        bevisited[li].append(url)
                    else:
                        link_url[li] = 1
                        bevisited[li] = [url]
                        urls.append(li)
        else:
            break
    Matric = visited
    creat_Matric(Matric)
    out(bevisited, 'Result_of_bevisited.txt', '], ', ']\n')
    out(visited, 'Result_of_visited.txt', '], ', ']\n')
    out(link_url, 'Result_of_times.txt', ',', '\n')


spider('http://www.nju.edu.cn/')
