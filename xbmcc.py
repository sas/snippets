#!/usr/bin/env python

# A small xbmc client that lists music files on the server.

import httplib
import json
import sys

class xbmc_rpc(object):
  def __init__(self, server, port=8080, url='/jsonrpc', username='', password=''):
    self.server = server
    self.port = port
    self.url = url
    self.username = username
    self.password = password

  def __getattr__(self, name):
    return self.call_proxy(name, self.server, self.port, self.url, self.username, self.password)

  class call_proxy(object):
    def __init__(self, method, server, port, url, username, password):
      self.method = method
      self.server = server
      self.port = port
      self.url = url
      self.username = username
      self.password = password

    def __getattr__(self, name):
      self.method = "%s.%s" % (self.method, name)
      return self

    def __call__(self, *args, **kwargs):
      command = {'jsonrpc': '2.0', 'method': self.method, 'id': 1}

      command['params'] = {}
      for kw in kwargs:
        command['params'][kw] = kwargs[kw]

      payload = json.dumps(command)

      headers = {'Content-Type': 'application/json-rpc; charset=utf-8'}

      if self.password != '':
        userpass = base64.encodestring('%s:%s' % (username, password))[:-1]
        headers['Authorization'] = 'Basic %s' % userpass

      conn = httplib.HTTPConnection(self.server, self.port)
      conn.request('POST', self.url, payload, headers)

      response = conn.getresponse()
      data = None
      if response.status == 200:
        data = response.read()

      conn.close()

      if data is not None:
        data = json.loads(data)
        if 'result' in data:
          return data['result']

      return None

def print_files(server, media='music'):
  def recurse(path):
    result = server.Files.GetDirectory(directory=path)

    if 'directories' in result:
      directories = result['directories']
      for d in directories:
        print(d['file'].encode('utf-8'))
        recurse(d['file'])

    if 'files' in result:
      files = result['files']
      for f in files:
        print(f['file'].encode('utf-8'))

  sources = server.Files.GetSources(media=media)
  for source in sources['shares']:
    recurse(source['file'])

if __name__ == '__main__':
  server = xbmc_rpc(sys.argv[1])
  print_files(server, media='music')
