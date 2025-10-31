import express from 'express'
import { getCard } from '../db/cards'
import { createState, getState, setState } from '../db/state'
import { createLogEntry, getLatestLogEntry, updateLogEntry } from '../db/accessLog'

const apiRouter = express.Router()

apiRouter.get("/validate/:uid", async (req, res) => {
    const card = await getCard(req.params.uid.toUpperCase())
    if (card === null || !card.active) {
        res.send("0,,")
        return
    }

    let state = await getState(card.uid)

    if (!state) {
        state = await createState(card.uid)
        if (!state) {
            res.send("0,,")
            return
        }
    }

    const dateTime = new Date();
    const time = dateTime.toISOString().slice(0,10)

    if (state.state) {
        res.send(`1,${card.name},TOUT: ${dateTime.toLocaleTimeString('en-GB', { hour12: false })}`)
        await setState(card.uid, false)
        const logEntry = await getLatestLogEntry(card.uid)
        if (!logEntry) {
            res.send(`0,,`)
            return
        }
        await updateLogEntry(logEntry.id, dateTime)
    } else {
        res.send(`1,${card.name},TIN: ${dateTime.toLocaleTimeString('en-GB', { hour12: false })}`)
        await setState(card.uid, true)
        await createLogEntry(card.uid, card.name, dateTime)
    }

})

export default apiRouter