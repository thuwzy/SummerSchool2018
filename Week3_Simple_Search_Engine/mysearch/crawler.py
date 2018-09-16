import re
import urllib.request
import urllib
import codecs
from urllib import request, parse
from bs4 import BeautifulSoup
import time
from urllib.error import HTTPError, URLError
import sys
from collections import deque
 
queue = deque()
visited = set()
visitedtitle=set()
 
url = 'http://news.sina.com.cn/'  # 入口页面, 可以换成别的
 
queue.append(url)
cnt = 0


class News(object):

    def __init__(self):

        self.url = None  #该新闻对应的url
        self.topic = None #新闻标题
        self.date = None #新闻发布日期
        self.content = None  #新闻的正文内容
        self.author = None  #新闻作者

def getNews(url):
    #获取页面所有元素
    print("In getNews!")
    html = request.urlopen(url).read().decode('utf-8', 'ignore')
    #解析
    soup = BeautifulSoup(html, 'html.parser')
    #获取信息
    
    if not(soup.find('div', {'class':'main-content w1240'})):
        print("1 Out!")
        return 
    news = News()  #建立新闻对象
    page = soup.find('div', {'class':'main-content w1240'})
    if not(page.find('h1', {'class':'main-title'})):
        print("2 Out!")
        return
    topic = page.find('h1', {'class':'main-title'}).get_text()  #新闻标题 
    news.topic = topic
    if not(page.find('div', {'id': 'article'})):
        print("3 Out!")
        return
    main_content = page.find('div', {'id': 'article'})   #新闻正文内容
    content = ''
    for p in main_content.select('p'):
        content = content + p.get_text()
    news.content = content
    news.url = url       #新闻页面对应的url

    if not(page.find('span', {'class': 'date'})):
        print("4 Out!")
        return
    date = page.find('span', {'class': 'date'}).get_text()
    news.date=date
    
    global visitedtitle
    if news.topic in visitedtitle:
    	print("Same News!")
    	return
    visitedtitle = visitedtitle | {news.topic}

    global cnt
    f = open('ifeng/news2.txt', 'a+', encoding='utf-8')
    f.write("cnt="+str(cnt))
    f.write('\n')
    f.write("title="+news.topic+'\n'+"date="+news.date+'\n'+"content="+news.content+'\n'+'\n')
    
    f.close()
    cnt=cnt+1
    print("Out getNews!"+str(cnt))




while queue:
  url = queue.popleft()  # 队首元素出队
  visited |= {url}  # 标记为已访问

  #if 'news.sina.com.cn/c/' in url:
  try: 
    getNews(url)
  except:
  	continue
 
  print('已经抓取: ' + str(cnt) + '   正在抓取 <---  ' + url)
  try:
    urlop = urllib.request.urlopen(url, timeout = 2)
  except:
    continue

  if 'html' not in urlop.getheader('Content-Type'):
    continue
 
  # 避免程序异常中止, 用try..catch处理异常
  try:
    
    data = urlop.read().decode('utf-8')
  except:
    continue
 
  # 正则表达式提取页面中所有队列, 并判断是否已经访问过, 然后加入待爬队列
  linkre = re.compile('href="(.+?)"')
  for x in linkre.findall(data):
    if 'http' in x and x not in visited and x not in queue:
        if 'news.sina.com.cn/' in x[0:25]:
            queue.append(x)
            print('加入队列 --->  ' + x)