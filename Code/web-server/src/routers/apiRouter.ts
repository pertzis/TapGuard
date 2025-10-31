import express from 'express'
import { getCard } from '../db/cards'

const apiRouter = express.Router()

apiRouter.get("/validate/:uid", async (req, res) => {
    const card = await getCard(req.params.uid)
    if (card === null || !card.active) {
        res.send("0,")
        return
    } 

    res.send(`1,${card.name}`)

})

export default apiRouter