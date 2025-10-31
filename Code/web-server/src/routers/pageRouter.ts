import express from 'express'
import type { NextFunction, Request } from 'express'
import { createCard, getCard, getCards, updateCard } from '../db/cards';
import { CardDto } from '../types/Card';
import { getAccessLog } from '../db/accessLog';

const pageRouter = express.Router()

pageRouter.get('/', async (req, res) => {
    const cards = await getCards()

    res.render("index.ejs", {
        snack: false,
        cards,
        active: req.path.split('/')[1]
    });
})

pageRouter.get('/new-card', (req, res) => {
    res.render("new-card.ejs", {
        active: req.path.split('/')[1]
    });
})

pageRouter.post('/new-card', async (req, res) => {

    const { name, uid, active } = req.body

    const card = await getCard(uid)
    if (card != null) {
        res.status(404).send("Card with UID already exists.")
        return
    }

    const newCard: CardDto = {
        name,
        uid: uid.toUpperCase(),
        active
    }

    if (await createCard(newCard)) {
        res.redirect('/')
    } else {
        res.status(500).send("Failed to create card.")
    }
   
})

pageRouter.get('/card-settings/:uid', async (req, res) => {
    const uid = req.params.uid

    const card = await getCard(uid)
    if (card != null) {
        res.render("card-settings.ejs", {
            active: req.path.split('/')[1],
            card
        });
    } else {
        res.status(404).send("Card not found.")
    }
   
})

pageRouter.post('/card-settings/:uid', async (req, res) => {
    const uid = req.params.uid
    const card = await getCard(uid)
    if (card === null) {
        res.status(404).send("Card not found.")
        return
    }

    const { name, uid: formUid, active } = req.body

    const updatedCard: CardDto = {
        name,
        uid: formUid.toUpperCase(),
        active
    }

    if (await updateCard(uid, updatedCard)) {
        res.redirect('/')
    } else {
        res.status(500).send("Failed to update card.")
    }
   
})

pageRouter.get('/access-log', async (req, res) => {

    const formatter = new Intl.DateTimeFormat("en-US", 
        { 
            dateStyle: "short", 
            timeStyle: "medium",
            hour12: false, 
        }
    )

    let accessLog =  await getAccessLog()
    const formattedAccessLog = accessLog.map(entry => ({
        ...entry,
        tin: formatter.format(entry.tin),
        tout: entry.tout === null ? "-" : formatter.format(entry.tout),
    }))
    res.render("access-log.ejs", {
        active: req.path.split('/')[1],
        accessLog: formattedAccessLog
    })
})

export default pageRouter