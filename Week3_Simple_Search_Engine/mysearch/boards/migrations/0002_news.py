# Generated by Django 2.1.1 on 2018-09-13 12:33

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('boards', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='News',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('title', models.CharField(max_length=255)),
                ('date', models.CharField(max_length=255)),
                ('contents', models.CharField(max_length=25555)),
                ('cnt', models.IntegerField()),
            ],
        ),
    ]
