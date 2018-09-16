from django.db import models
from django.contrib.auth.models import User

# 在翻译这篇文章的时候，使用的是python 3.6.4， Django 2.0.2
# ForeignKey中有一个on_delete参数

class Board(models.Model):
    name = models.CharField(max_length=30, unique=True)
    description = models.CharField(max_length=100)

    def __str__(self):
        return self.name

class Topic(models.Model):
    subject = models.CharField(max_length=255)
    last_updated = models.DateTimeField(auto_now_add=True)
    board = models.ForeignKey(Board, models.CASCADE, related_name='topics')
    starter = models.ForeignKey(User, models.CASCADE, related_name='topics')

class Post(models.Model):
    message = models.TextField(max_length=4000)
    topic = models.ForeignKey(Topic, models.CASCADE, related_name='posts')
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(null=True)
    created_by = models.ForeignKey(User, models.CASCADE, related_name='posts')
    updated_by = models.ForeignKey(User, models.CASCADE, null=True, related_name='+')

class News(models.Model):
    title = models.CharField(max_length=255)
    date = models.CharField(max_length=255)
    contents = models.CharField(max_length=25555)
    cnt = models.IntegerField()