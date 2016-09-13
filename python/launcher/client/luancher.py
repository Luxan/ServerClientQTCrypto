#!/usr/bin/python
import urllib2
from subprocess import call
from sys import platform as _platform
import zipfile

def download(destination_folder_name, download_url):
  file_name = download_url.split('/')[-1]
  try:
    u = urllib2.urlopen(download_url)
  except URLError:
    print "Cannot open url!"

  meta = u.info()
  file_size = int(meta.getheaders("Content-Length")[0])
  print "Downloading: %s Bytes: %s" % (file_name, file_size)
  
  file_size_dl = 0
  block_sz = 8192
  with open(destination_folder_name + file_name, 'rwb') as f:
    while True:
        buffer = u.read(block_sz)
        if not buffer:
            break
    
        file_size_dl += len(buffer)
        f.write(buffer)
        status = r"%10d  [%3.2f%%]" % (file_size_dl, file_size_dl * 100. / file_size)
        status = status + chr(8)*(len(status)+1)
        print status,
        
  return file_name

def md5(fname):
    hash_md5 = hashlib.md5()
    with open(fname, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.hexdigest()

veryfication_file_name = "verify"
url = ""
download_path = ""
install_path = ""
f = ""
file_md5 = ""

if _platform == "linux" or _platform == "linux2":
  # linux
  download_path = "/tmp/"
  url = "https://download.gorokh.eu/download/linux.zip"
elif _platform == "darwin":
  # MAC OS X
  download_path = "/tmp/"
  url = "https://download.gorokh.eu/download/mac.zip"
elif _platform == "win32":
  # Windows
  download_path = call(["echo", "%Temp%"]) + "\\"
  url = "https://download.gorokh.eu/download/windows.zip"
#download file from web site

file_name = download(download_path, url)
#unzip file and install to specified path
with open(download_path + file_name, 'rwb') as f:
  z = zipfile.ZipFile(f)
  for name in z.namelist():
      outfile = open(name, 'wb')
      outfile.write(install_path + z.read(name))
      outfile.close()
#veryfy that all files are correct
corrupted_file = ""
is_corrupted = false
try:
  with open(install_path + veryfication_file_name, "r") as f:
    for line in f:
        corrupted_file = line.split(':')[0]
        if md5(corrupted_file) != line.split(':')[1]:
          print "Error: " + corrupted_file + " is corrupted!"
          is_corrupted = true
except IOError:
    # verification file do not exist
    if corrupted_file == "":
      print "Error: verification file do not exist"
      is_corrupted = true
    else:
      print "Error: " + corrupted_file + " is corrupted!"
      is_corrupted = true

if is_corrupted:
  sys.exit()

