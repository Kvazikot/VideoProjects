import requests 
import json

url = "https://texttospeech.googleapis.com/v1beta1/text:synthesize"

text = "This is a text"

data = {
        "input": {"text": text},
        "voice": {"name":  "fr-FR-Wavenet-A", "languageCode": "fr-FR"},
        "audioConfig": {"audioEncoding": "MP3"}
      };

headers = {"content-type": "application/json", "X-Goog-Api-Key": "AIzaSyDEe9sKR2x8M68zF_aFvTrHY63gjkHQSGk" }

r = requests.post(url=url, json=data, headers=headers)
content = json.loads(r.content)
print(content)
# The response's audio_content is binary.
# with open('output.mp3', 'wb') as out:
    # # Write the response to the output file.
    # out.write(content)
    # print('Audio content written to file "output.mp3"')