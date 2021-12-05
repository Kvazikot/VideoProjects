import win32com.client as wincl
import re
f = open('text.txt')

# list voices
speak = wincl.Dispatch("SAPI.SpVoice")
print(len(speak.GetVoices()))
for i in range(0,len(speak.GetVoices()),1):
    print(speak.GetVoices().Item(i).GetDescription())

# parse abzaci
text = f.read()
abzaci = re.split(r'[\n]{2,}', text)
n = len(abzaci)-1
for i in range(0, n, 1):
    abzaci[i] = re.sub(r'[\n]+',' ',abzaci[i])
    print(f'{i} abzac\n' + abzaci[i])
    file = f'part_{i}.wav'
    print(file)
    fstream = wincl.Dispatch("SAPI.SpFileStream")
    fstream.Open(file, 3, 0)
    speak.AudioOutputStream = fstream
    speak.Speak(abzaci[i])


