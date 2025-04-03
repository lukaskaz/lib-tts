# lib-tts
Library for text to voice conversions
<br><br>
**Example of the output (w/o and w/ debug logs):**<br>
pi@raspberrypi:\~/git/lib-tts/build $ ./examples/googlebasic/googlebasic 0<br>
[INFO] Created gbasic tts [lang/gender/idx]: pl/male/1<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot create already existing directory: 'audio'<br>
[INFO] Created gbasic tts [lang/gender/idx]: en/male/1<br>
[INFO] Released gbasic tts [lang/gender/idx]: en/male/1<br>
[WARN] Not removing previously existed directory: 'audio'<br>
[INFO] Released gbasic tts [lang/gender/idx]: pl/male/1<br>
<br>
pi@raspberrypi:\~/git/lib-tts/build $ ./examples/googlebasic/googlebasic 1<br>
[DBG ] Created directory: 'audio'<br>
[INFO] Created gbasic tts [lang/gender/idx]: pl/male/1<br>
[DBG ] Requested text to speak: 'Jestem twoim zwykłym asystentem, co mam zrobić?'<br>
[DBG ] Requested text to speak: 'Jestem twoim asynk asystentem, co mam zrobić?'<br>
[DBG ] Requested text to speak: 'Jestem twoim asynk asystentem, co mam zrobić?'<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[DBG ] Requested text to speak: 'Jestem twoim zwykłym asystentem, co mam zrobić?'<br>
[WARN] Cannot create already existing directory: 'audio'<br>
[INFO] Created gbasic tts [lang/gender/idx]: en/male/1<br>
[DBG ] Requested text to speak: 'Hi, this is second speech!'<br>
[INFO] Released gbasic tts [lang/gender/idx]: en/male/1<br>
[WARN] Not removing previously existed directory: 'audio'<br>
[DBG ] Requested text to speak: 'Tschüss, wie gehts du?'<br>
[DBG ] Text spoken as de/male/1<br>
[DBG ] Requested text to speak: 'To wszystko, dzięki :)'<br>
[INFO] Released gbasic tts [lang/gender/idx]: pl/male/1<br>
[DBG ] Removed directory: 'audio'<br>
<br>
pi@raspberrypi:\~/git/lib-tts/build $ ./examples/googleapi/googleapi 0<br>
[INFO] Created gapi tts [langcode/langname/gender]: pl-PL/pl-PL-Standard-E/FEMALE<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot create already existing directory: 'audio'<br>
[INFO] Created gapi tts [langcode/langname/gender]: en-US/en-US-Standard-C/FEMALE<br>
[INFO] Released gapi tts [langcode/langname/gender]: en-US/en-US-Standard-C/FEMALE<br>
[WARN] Not removing previously existed directory: 'audio'<br>
[INFO] Released gapi tts [langcode/langname/gender]: pl-PL/pl-PL-Standard-E/FEMALE<br>
<br>
pi@raspberrypi:\~/git/lib-tts/build $ ./examples/googleapi/googleapi 1<br>
[DBG ] Created directory: 'audio'<br>
[INFO] Created gapi tts [langcode/langname/gender]: pl-PL/pl-PL-Standard-E/FEMALE<br>
[DBG ] Requested text to speak: 'Jestem twoim zwykłym asystentem, co mam zrobić?'<br>
[DBG ] Written data of size: 35072, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asynk asystentem, co mam zrobić?'<br>
[DBG ] Written data of size: 34816, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asynk asystentem, co mam zrobić?'<br>
[DBG ] Written data of size: 34816, to file: 'audio/playback.mp3'<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[DBG ] Requested text to speak: 'Jestem twoim zwykłym asystentem, co mam zrobić?'<br>
[DBG ] Written data of size: 35072, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asystentem, co mam zrobić?'<br>
[DBG ] Text spoken as pl-PL/pl-PL-Standard-E/FEMALE<br>
[DBG ] Written data of size: 30208, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asystentem, co mam zrobić?'<br>
[DBG ] Text spoken as pl-PL/pl-PL-Standard-A/FEMALE<br>
[DBG ] Written data of size: 30208, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asystentem, co mam zrobić?'<br>
[DBG ] Text spoken as pl-PL/pl-PL-Standard-D/FEMALE<br>
[DBG ] Written data of size: 30208, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asystentem, co mam zrobić?'<br>
[DBG ] Text spoken as pl-PL/pl-PL-Standard-B/MALE<br>
[DBG ] Written data of size: 30464, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asystentem, co mam zrobić?'<br>
[DBG ] Text spoken as pl-PL/pl-PL-Standard-C/MALE<br>
[DBG ] Written data of size: 30464, to file: 'audio/playback.mp3'<br>
[WARN] Cannot create already existing directory: 'audio'<br>
[INFO] Created gapi tts [langcode/langname/gender]: en-US/en-US-Standard-C/FEMALE<br>
[DBG ] Requested text to speak: 'Hi, this is second speech!'<br>
[DBG ] Written data of size: 22784, to file: 'audio/playback.mp3'<br>
[INFO] Released gapi tts [langcode/langname/gender]: en-US/en-US-Standard-C/FEMALE<br>
[WARN] Not removing previously existed directory: 'audio'<br>
[DBG ] Requested text to speak: 'Tschüss, wie gehts du?'<br>
[DBG ] Text spoken as de-DE/de-DE-Standard-C/FEMALE<br>
[DBG ] Written data of size: 18944, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'To wszystko, dzięki :)'<br>
[DBG ] Written data of size: 29696, to file: 'audio/playback.mp3'<br>
[INFO] Released gapi tts [langcode/langname/gender]: pl-PL/pl-PL-Standard-E/FEMALE<br>
[DBG ] Removed directory: 'audio'<br>
<br>
pi@raspberrypi:\~/git/lib-tts/build $ ./examples/googlecloud/googlecloud 0<br>
[INFO] Created gcloud tts [langcode/langname/gender]: pl-PL/pl-PL-Standard-E/female<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot create already existing directory: 'audio'<br>
[INFO] Created gcloud tts [langcode/langname/gender]: en-US/en-US-Standard-C/female<br>
[INFO] Released gcloud tts [langcode/langname/gender]: en-US/en-US-Standard-C/female<br>
[WARN] Not removing previously existed directory: 'audio'<br>
[INFO] Released gcloud tts [langcode/langname/gender]: pl-PL/pl-PL-Standard-E/female<br>
<br>
pi@raspberrypi:\~/git/lib-tts/build $ ./examples/googlecloud/googlecloud 1<br>
[DBG ] Created directory: 'audio'<br>
[INFO] Created gcloud tts [langcode/langname/gender]: pl-PL/pl-PL-Standard-E/female<br>
[DBG ] Requested text to speak: 'Jestem twoim zwykłym asystentem, co mam zrobić?'<br>
[DBG ] Written data of size: 154342, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asynk asystentem, co mam zrobić?'<br>
[DBG ] Written data of size: 152364, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asynk asystentem, co mam zrobić?'<br>
[DBG ] Written data of size: 152364, to file: 'audio/playback.mp3'<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[WARN] Cannot speak text: 'Jestem twoim zwykłym asystentem, co mam zrobić?', tts in use<br>
[DBG ] Requested text to speak: 'Jestem twoim zwykłym asystentem, co mam zrobić?'<br>
[DBG ] Written data of size: 154342, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asystentem, co mam zrobić?'<br>
[DBG ] Text spoken as pl-PL/pl-PL-Standard-E/female<br>
[DBG ] Written data of size: 132374, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asystentem, co mam zrobić?'<br>
[DBG ] Text spoken as pl-PL/pl-PL-Standard-A/female<br>
[DBG ] Written data of size: 132374, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asystentem, co mam zrobić?'<br>
[DBG ] Text spoken as pl-PL/pl-PL-Standard-D/female<br>
[DBG ] Written data of size: 132374, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asystentem, co mam zrobić?'<br>
[DBG ] Text spoken as pl-PL/pl-PL-Standard-B/male<br>
[DBG ] Written data of size: 133366, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'Jestem twoim asystentem, co mam zrobić?'<br>
[DBG ] Text spoken as pl-PL/pl-PL-Standard-C/male<br>
[DBG ] Written data of size: 133366, to file: 'audio/playback.mp3'<br>
[WARN] Cannot create already existing directory: 'audio'<br>
[INFO] Created gcloud tts [langcode/langname/gender]: en-US/en-US-Standard-C/female<br>
[DBG ] Requested text to speak: 'Hi, this is second speech!'<br>
[DBG ] Written data of size: 98194, to file: 'audio/playback.mp3'<br>
[INFO] Released gcloud tts [langcode/langname/gender]: en-US/en-US-Standard-C/female<br>
[WARN] Not removing previously existed directory: 'audio'<br>
[DBG ] Requested text to speak: 'Tschüss, wie gehts du?'<br>
[DBG ] Text spoken as de-DE/de-DE-Standard-C/female<br>
[DBG ] Written data of size: 81148, to file: 'audio/playback.mp3'<br>
[DBG ] Requested text to speak: 'To wszystko, dzięki :)'<br>
[DBG ] Written data of size: 129514, to file: 'audio/playback.mp3'<br>
[INFO] Released gcloud tts [langcode/langname/gender]: pl-PL/pl-PL-Standard-E/female<br>
[DBG ] Removed directory: 'audio'<br>
