import serial
import pyaudio
import speech_recognition as sr
import google.generativeai as genai

# --- CONFIGURATION ---
ESP32_PORT = "COM3" # Change to your port
BAUD_RATE = 115200
GEMINI_API_KEY = "YOUR_GEMINI_API_KEY"

# Initialize Gemini
genai.configure(api_key=GEMINI_API_KEY)
model = genai.GenerativeModel('gemini-1.5-flash')

# Initialize Serial
try:
    # We use a very fast timeout so we can constantly check the audio stream
    esp32 = serial.Serial(ESP32_PORT, BAUD_RATE, timeout=0.01)
    print(f"Connected to ESP32 on {ESP32_PORT}")
except Exception as e:
    print(f"Connection Failed: {e}")
    exit()

# Audio Settings
CHUNK = 1024
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100
p = pyaudio.PyAudio()

def process_audio_and_ask(audio_frames):
    print("Processing audio...")
    # Convert the raw recorded chunks into an AudioData object
    raw_audio = b''.join(audio_frames)
    audio_data = sr.AudioData(raw_audio, RATE, 2) # 2 bytes = 16 bit
    recognizer = sr.Recognizer()
    
    try:
        # Convert Speech to Text
        text = recognizer.recognize_google(audio_data)
        print(f"You said: {text}")
        
        # Ask Gemini
        prompt = text + " (Keep your response to 15 words or less.)"
        response = model.generate_content(prompt)
        answer = response.text.replace('\n', ' ').strip()
        print(f"Gemini: {answer}")
        
        # Send Answer to ESP32
        esp32.write((answer + '\n').encode('utf-8'))
        
    except sr.UnknownValueError:
        print("Could not understand audio.")
        esp32.write(("I didn't catch that.\n").encode('utf-8'))
    except Exception as e:
        print(f"Error: {e}")
        esp32.write(("Error getting answer.\n").encode('utf-8'))


# --- Main Loop ---
is_recording = False
frames = []
stream = None

print("\nReady! Hold the button on your ESP32 to talk.")

try:
    while True:
        # 1. Check for signals from ESP32
        if esp32.in_waiting > 0:
            msg = esp32.readline().decode('utf-8').strip()
            
            if msg == "START_MIC":
                print("\n[🔴 RECORDING...] Talk now!")
                frames = [] # Clear old audio
                is_recording = True
                # Open the microphone stream
                stream = p.open(format=FORMAT, channels=CHANNELS, rate=RATE, input=True, frames_per_buffer=CHUNK)
            
            elif msg == "STOP_MIC" and is_recording:
                print("[⏹️ STOPPED] Sending to Gemini...")
                is_recording = False
                stream.stop_stream()
                stream.close()
                # Send the collected audio to be processed
                process_audio_and_ask(frames)
                print("\nReady! Hold button to talk.")

        # 2. If we are currently holding the button down, continuously grab audio chunks
        if is_recording and stream is not None:
            # exception_on_overflow=False prevents crashes if the mic gets overloaded
            data = stream.read(CHUNK, exception_on_overflow=False)
            frames.append(data)

except KeyboardInterrupt:
    print("Exiting...")
finally:
    p.terminate()
    esp32.close()