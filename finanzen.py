from lxml import html
import requests

page = requests.get('http://www.boerse-frankfurt.de/aktie/Oracle-Aktie')
tree = html.fromstring(page.content)

kurs = tree.xpath('//div[@field="last"]/text()')

print kurs
