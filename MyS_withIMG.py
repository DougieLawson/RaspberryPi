#!/usr/bin/python
import os
import sys
import MySQLdb
import tweepy

db = MySQLdb.connect(host="raspberrypi", 
                     user="mySQLuser",
                      passwd="mySQLpasswd",
                      db="mySQLdb") 
cur = db.cursor() 

cur.execute("SELECT consumer_key, consumer_secret, access_token, access_token_secret FROM Oauth where twitter_id = 'myTweepyID';")

for row in cur.fetchall() :
    Ckey = row[0]
    Csec = row[1]
    Akey = row[2]
    Asec = row[3]

auth = tweepy.OAuthHandler(Ckey, Csec)
auth.set_access_token(Akey, Asec)
twapi = tweepy.API(auth)

fn = os.path.abspath(sys.argv[1])
status = sys.argv[2]

twapi.update_with_media(fn, status=status)
