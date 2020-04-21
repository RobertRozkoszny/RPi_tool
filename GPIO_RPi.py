import urllib.parse
import requests
import pprint

#-jezeli jako stan P1 wpisze sie '?'to program zwraca stany wszystkich GPIO

main_url = 'http://192.168.1.124/api/project_1/hello?'

P0 = input('P0:')
P1 = input('P1:')
P2 = input('P2:')
P3 = input('P3:')
P4 = input('P4:')
P5 = input('P5:')
P6 = input('P6:')
P7 = input('P7:')

url = main_url  + urllib.parse.urlencode({'P0': P0,'P1': P1,'P2': P2,'P3': P3,'P4': P4,'P5': P5,'P6': P6,'P7': P7})

#stuff= ['P0','P1','P2','P3','P4','P5','P6' ,'P7']


print(url)
json_data = requests.get(url).json()
print(json_data)


