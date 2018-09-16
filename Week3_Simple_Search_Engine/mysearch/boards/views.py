from django.shortcuts import render
from django.contrib.auth.models import User
from django.shortcuts import render, redirect, get_object_or_404
from .models import Board, Topic, Post,News
import json
import jieba
import re
import time
from random import randint
import random
from django.core import paginator
def home(request):
    boards = Board.objects.all()   
    if request.method == 'POST':
        search = request.POST['search']
        sTime = request.POST.get('startTime','')
        eTime = request.POST.get('endTime','')
        return redirect('info', index=1,tsearch=search+'-'+sTime+'-'+eTime)

        #return render(request,'info.html',context)
        '''
        search = request.POST['search']
        if search is '':
            return render(request, 'home.html', {'boards': boards})              
        f=open("../index.pkl",'r',encoding='utf-8')
        a=json.load(f)
        f.close()
        try:
            ans=a[search]
        except:
            ans=[]
        results=[]
        cntt=0
        for num in ans:
            cntt=cntt+1
            if cntt>15:
                break
            news = News.objects.get(cnt=num)
            results.append(news)
        '''
        '''
        message = request.POST['message']

        user = User.objects.first()  # TODO: 临时使用一个账号作为登录用户

        topic = Topic.objects.create(
            subject=subject,
            board=board,
            starter=user
        )

        post = Post.objects.create(
            message=message,
            topic=topic,
            created_by=user
        )
        '''
        #return render(request, 'result.html', {'search': search,'results': results}) # TODO: redirect to the created topic page
    return render(request, 'home.html', {'boards': boards})

def board_topics(request, pk):
    board = get_object_or_404(Board, pk=pk)
    # 我不懂为什么pk也有用……囧
    return render(request, 'topics.html', {'board': board})

def get_details(request, cnt):
    news = News.objects.get(cnt=cnt)
    nlist=list(jieba.cut(news.title))
    f=open("../index.pkl",'r',encoding='utf-8')
    a=json.load(f)
    f.close()
    recomments=[]
    cntt=0
    for i in nlist:
        try:
            tmp=len(a[i])
            ans=a[i][randint(0,tmp)]
            tmpnews=News.objects.get(cnt=ans)
            if tmpnews.title!=news.title:
                recomments.append(tmpnews)
                cntt=cntt+1
            if cntt>=10:
                break
        except:
            pass
    return render(request, 'details.html', {'news': news,'recomments': recomments})
    #boards = Board.objects.all()
    #return render(request, 'home.html', {'boards': boards})

def new_topic(request, pk):
    board = Board.objects.get(pk=pk)
    if request.method == 'POST':
        subject = request.POST['subject']
        message = request.POST['message']

        user = User.objects.first()  # TODO: 临时使用一个账号作为登录用户

        topic = Topic.objects.create(
            subject=subject,
            board=board,
            starter=user
        )

        post = Post.objects.create(
            message=message,
            topic=topic,
            created_by=user
        )
        return redirect('board_topics', pk=board.pk)  # TODO: redirect to the created topic page
    return render(request, 'new_topic.html', {'board': board})

with open("../index.pkl",'r',encoding='utf-8') as f:
    a=json.load(f)

    
def analyze(t):
    return list(map(int,re.compile('[0-9]+').findall(t)))[:3]

def info(request,index,tsearch):
    tlist=tsearch.split('-')
    # 获取所有的的数据，list是一个列表，包含有所有数据对应的实例对象
    startTime=time.time()

    print(tsearch,tlist)

    search,sTime,eTime = tlist

    global a

    try:
        ans=a[search]
    except:
        ans=[]
    results=[]
    cntt=0
    for num in ans:        
        if cntt>15:
            break
        cntt=cntt+1
        news = News.objects.get(cnt=num)
        results.append(news)
    # 使用Paginator方法返回一个分页的对象
    # 这个对象包括所有数据，分页的情况
    #print(analyze(results[0].date),results[0].date,type(results[0].date),sTime)
    results=sorted(results,key=lambda x:analyze(x.date))
    if sTime:
        sTime=analyze(sTime)
        results=[i for i in results if sTime<=analyze(i.date)]
    if eTime:
        eTime=analyze(eTime)
        results=[i for i in results if eTime>=analyze(i.date)]
    #random.shuffle(results)
    pag = paginator.Paginator(results,5)
    # 使用此判断语句是为了在用户跳转www.xxx.com/info/时也能访问第一页
    if index=='':
        index=1
    # 返回指定（index）页的数据，用于呈现在指定页上
    page = pag.page(index)
    # 构造上下文，以便html文件中能调用对应页的数据
    endTime=time.time()
    time1=str(endTime-startTime)
    print("Time consuming:"+str(endTime-startTime))

    context = {
        'page':page,
        'tsearch':tsearch,
        'search':search,
        'time1':time1,
        'cntt':len(results),
    }

    
    return render(request,'info.html',context)

def index(request,index):

    results=list(News.objects.all())
    num=len(results)
    search=''
    # 使用Paginator方法返回一个分页的对象
    # 这个对象包括所有数据，分页的情况
    random.shuffle(results)
    pag = paginator.Paginator(results,300)
    # 使用此判断语句是为了在用户跳转www.xxx.com/info/时也能访问第一页
    # 返回指定（index）页的数据，用于呈现在指定页上
    if index=='':
        index=1
    if index=='1/':
        index=1
    page = pag.page(index)
    # 构造上下文，以便html文件中能调用对应页的数据
    context = {
        'page':page,
        'search':search,
        'num':num,
    }

    return render(request,'index.html',context)