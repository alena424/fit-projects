import { CellType, RuleType } from "../models/cellModel";

export const randomArray = (boardWidth: number): CellType[] => Array(boardWidth).fill(undefined).map(() => {
    const randomNumber = Math.floor(Math.random() * 10);
    if (randomNumber % 2 === 0) {
        return { active: true }
    }
    return { active: false }
})

export const randomRule = (count: number): RuleType[] => Array(count).fill(0).map((val, index) =>
    ({
        value: Math.floor(Math.random() * 10) % 2 === 0,
        index: index
    }))

export const emptyRule = (count: number): RuleType[] => Array(count).fill(0).map((val, index) =>
    ({
        value: false,
        index: index
    }))

export const emptyArray = (boardWidth: number): CellType[] => Array(boardWidth).fill(undefined).map(() => ({
    active: false
}))

export const compareArrays = (arr1: CellType[], arr2: CellType[]) => {
    for (let i = 0; i < arr1.length; i++) {
        if (arr1[i].active !== arr2[i].active) {
            return false
        }
    }
    return true
}

const findRule = (index: number, row: CellType[], neighborhood: number): number => {
    let j = 1;
    let ruleIndex = 0;
    for (let idx = index + neighborhood; idx >= index - neighborhood; idx--) {
        console.log("text", j)
        ruleIndex += (row[idx] !== undefined && row[idx].active ? 1 : 0) * j;
        j = j + j;
    }
    return ruleIndex;
}

const applyRule = (index: number, row: CellType[], neighborhood: number, rules: RuleType[]): boolean => {
    const ruleIndex = findRule(index, row, neighborhood);
    console.log("applyRule", ruleIndex, row, index, rules);

    if (rules[ruleIndex] === undefined){
        alert("Please reinit the rules");
        return false;
    }
    return rules[ruleIndex].value;

}

// Go through all the row and evaluate next iteration (by looking at the neighbors of each cell)
export const generateNewRow = (row: CellType[], neighborhood: number, rules: RuleType[]): CellType[] =>
    row.map((cell, index) => ({ ...cell, active: applyRule(index, row, neighborhood, rules) })
)
