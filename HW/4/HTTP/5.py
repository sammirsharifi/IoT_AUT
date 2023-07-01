from fastapi import FastAPI, Request
import json


class Music:
    def __init__(self, genre, duration, name, singer):
        self.genre = genre
        self.duration = duration
        self.name = name
        self.singer = singer

    def get_info(self):
        info = "name:" + self.name + "\n" + \
               "singer:" + self.singer + "\n" + \
               "genre:" + self.genre + "\n" + \
               "duration:" + self.duration + "\n"
        return info


musics = {}
playList = []
playlist_status = ()


def add_music(info):
    try:

        musics[info["name"]] = Music(genre=info["genre"], duration=info["duration"], name=info["name"],
                                     singer=info["singer"])

        return info["name"] + " was added to the list."
    except Exception:
        return "request failed."


def remove_music(info):
    if info["name"] in musics:
        music = musics.pop(info["name"])
        return music.name + " was deleted."
    else:
        return info["name"] + " not found."


def check_music(info, musicList=True, plist=True):
    if info["name"] not in musics and musicList:
        return info["name"] + " not found."
    elif info["name"] not in playList and plist:
        return info["name"] + " dose not exist in playList."
    else:
        return "OK"


def addToPlayList(info):
    result = check_music(info, plist=False)
    if result != "OK":
        return result
    else:
        playList.append(info["name"])
        return info["name"] + " was added to playList successfully."


def removeFromPlayList(info):
    result = check_music(info)
    if result != "OK":
        return result
    else:
        playList.remove(info["name"])
        return info["name"] + " was deleted from playList successfully."


def run_music(info):
    global playlist_status
    result = check_music(info)
    if result != "OK":
        return result
    else:
        playlist_status = (info["name"], True)
        return "running of " + info["name"] + " music began."


def stop_music():
    global playlist_status
    if playlist_status == ():
        return "no music is running."
    else:
        music_name = playlist_status[0]
        playlist_status = (music_name, False)
        return music_name + " was stopped."


def get_plist_status():
    if playlist_status == ():
        return "no music is running."
    if playlist_status[1]:
        return playlist_status[0] + " is running."
    else:
        return playlist_status[0] + " was selected but is stopped. "


app = FastAPI()


@app.post("/addMusic")
async def root(request: Request):
    info = await request.json()
    return add_music(info)

@app.get("/getMusics")
async def root():
    return {str(key): value.get_info() for key, value in musics.items()}

@app.post("/removeMusic")
async def root(request: Request):
    info = await request.json()
    return remove_music(info)

@app.post("/addToPlayList")
async def root(request: Request):
    info = await request.json()
    return addToPlayList(info)

@app.post("/removeFromPlayList")
async def root(request: Request):
    info = await request.json()
    return removeFromPlayList(info)

@app.post("/runMusic")
async def root(request: Request):
    info = await request.json()

    return run_music(info)


@app.get("/stopMusic")
async def root():
    return stop_music()


@app.get("/playListStatus")
async def root():
    return get_plist_status()
