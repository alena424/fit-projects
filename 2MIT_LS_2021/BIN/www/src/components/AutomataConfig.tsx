import React from "react";
import { SetNumberFunction } from "../models/cellModel";

type AutomataConfigProps = {
    boardWidth: number
    setBoardWidth: SetNumberFunction
    neighborhood: number
    setNeighborhood: SetNumberFunction
    delay: number
    setDelay: SetNumberFunction
    maxNumberSteps: number
    setMaxNumberSteps: SetNumberFunction
}

const AutomataConfig: React.FC<AutomataConfigProps> = (props) => {
    const {
        boardWidth,
        setBoardWidth,
        neighborhood,
        setNeighborhood,
        delay,
        setDelay,
        maxNumberSteps,
        setMaxNumberSteps,
    } = props;
    const selectNeighborhood = (e: any) => {
        const val = e.target.value
        setNeighborhood(parseInt(val))
    }

    return (
        <div className={"row"}>
            <div className={"configItem field"}>
                <div className="field-label is-normal">
                    <label className="label">Number of rows: </label>
                </div>

                <div className="field-body">
                    <input
                        className="input shortInput"
                        pattern="^\d*$"
                        value={boardWidth}
                        onChange={(val) => {
                            const rows = parseInt(val.currentTarget.value)
                            if (rows >= 1 && rows <= 100) {
                                setBoardWidth(rows)
                            }
                        }}/>
                </div>
            </div>
            <div className={"configItem"}>
                <label className="label">Neighborhood: </label>
                <div className="select">
                    <select name="sur" id="neighborhood" onChange={selectNeighborhood}
                            value={neighborhood.toString()}>
                        <option value="1">1-neighborhood</option>
                        <option value="2">2-neighborhood</option>
                        <option value="3">3-neighborhood</option>
                        <option value="4">4-neighborhood</option>
                    </select>
                </div>
            </div>
            <div className={"configItem"}>
                <label className="label">Delay: </label>
                <input
                    className="input shortInput"
                    pattern="^\d*$"
                    defaultValue={delay}
                    type="number"
                    onChange={(val) => {
                        const delay = parseInt(val.currentTarget.value)
                        setDelay(delay)
                    }}/>
            </div>
            <div className={"configItem"}>
                <label className="label">Maximum number of steps: </label>
                <input
                    className="input shortInput"
                    pattern="^\d*$"
                    value={maxNumberSteps}
                    type="number"
                    onChange={(val) => {
                        const maxSteps = parseInt(val.currentTarget.value)
                        setMaxNumberSteps(maxSteps)
                    }}/>
            </div>

        </div>
    )
}

export default AutomataConfig;
