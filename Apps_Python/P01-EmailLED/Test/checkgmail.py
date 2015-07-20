import imaplib
obj = imaplib.IMAP4_SSL('imap.gmail.com','993')
obj.login('dimiter.q@gmail.com','integer!1')
obj.select()
obj.search(None,'UnSeen')
print len(obj.search(None, 'UnSeen')[1][0].split())
