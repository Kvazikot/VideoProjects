from google.cloud import speech_v1
from google.cloud import speech_v1 as speech
from google.cloud.speech_v1 import types

import (
        "context"
        "fmt"

        "cloud.google.com/go/pubsub"
        "google.golang.org/api/option"
)

// apiKey shows how to use an API key to authenticate.
func apiKey() error {
        client, err := pubsub.NewClient(context.Background(), "your-project-id", option.WithAPIKey("api-key-string"))
        if err != nil {
                return fmt.Errorf("pubsub.NewClient: %v", err)
        }
        defer client.Close()
        // Use the authenticated client.
        _ = client

        return nil
}


#client = speech_v1.SpeechClient()
#encoding = types.RecognitionConfig.AudioEncoding.FLAC
#sample_rate_hertz = 44100
#language_code = 'en-US'
#config = {'encoding': encoding, 'sample_rate_hertz': sample_rate_hertz, 'language_code': language_code}
#uri = 'gs://bucket_name/file_name.flac'
#audio = {'uri': uri}

#response = client.recognize(config, audio)