import { pool } from "../db"
import { LogEntry, LogEntryDto } from "../types/LogEntryDto"


export async function getAccessLog(): Promise<LogEntryDto[]> {
    const [rows] = await pool.query(
        'SELECT * FROM access_log ORDER BY tin DESC'
    )

    const logEntries: LogEntryDto[] = rows.map((row: LogEntry) => (
        {
            uid: row.uid,
            name: row.name,
            tin: row.tin,
            tout: row.tout
        }
    ))

    return logEntries

}

export async function getLatestLogEntry(uid: string): Promise<LogEntryDto | null> {
    const [rows] = await pool.query(
        'SELECT * FROM access_log WHERE uid = ? ORDER BY tin DESC',
        [uid]
    )

    if (rows[0]) {
        return {
            id: rows[0].id,
            uid: rows[0].uid,
            name: rows[0].name,
            tin: rows[0].tin,
            tout: rows[0].tout
        }
    }
    return null

}
export async function createLogEntry(uid: string, name: string, tin: Date) {
    let [result] = await pool.query(
        'INSERT INTO access_log (uid, name, tin) VALUES(?, ?, ?)',
        [uid, name, tin]
    )

    const info = result as any;

    if (info.affectedRows === 0) {
        return null
    }
    return {
        uid,
        state: false
    }
}

export async function updateLogEntry(id: number, tout: Date): Promise<boolean> {
    let [result] = await pool.query(
        'UPDATE access_log SET tout = ? WHERE id = ?',
        [tout, id]
    )

    const info = result as any;

    if (info.affectedRows === 0) {
        return false
    }
    return true
}
