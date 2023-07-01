import random
from fastapi import FastAPI

app = FastAPI()
@app.get("/randNum")
async def root():
    return {"number": str(random.randint(0,100))}