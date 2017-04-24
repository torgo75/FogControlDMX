from lxml import html
import requests

oracle = requests.get('http://www.boerse-frankfurt.de/aktie/Oracle-Aktie')
uniglobal = requests.get('http://www.boerse-frankfurt.de/fonds/UniGlobal--net-')

tree = html.fromstring(uniglobal.content)

wp = tree.xpath('/html/head/title/text()')
kurs = tree.xpath('//div[@item="X00000A0200DE0009750273"]/text()')

print wp
print kurs
