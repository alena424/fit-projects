export type CellType = {
    active: boolean
}

export type RuleType = {
    index: number
    value: boolean
}

export type SetNumberFunction = (a: number) => void

export type OnClickCell = (cell: CellType, key: number) => void
