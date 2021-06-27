import Rule from "./Rule";
import React from "react";
import { CellType, RuleType } from "../models/cellModel";
import { emptyRule, randomRule } from "../utils/cellUtils";

type ConfigRulesProps = {
    rules: RuleType[]
    ruleLength: number
    setRules: (a: RuleType[]) => void
}

const ConfigRulesSection: React.FC<ConfigRulesProps> = (props) => {
    const { rules, ruleLength, setRules } = props

    const numberOfRules = rules.length
    const clearRules = () => {
        setRules(emptyRule(numberOfRules))
    }
    const randomInitRules = () => {
        setRules(randomRule(numberOfRules))
    }

    const onClick = (cell: CellType, key: number) => {
        const newRules = [...rules]
        newRules[key] = {...newRules[key], value: ! newRules[key].value}
        setRules(newRules)
    }
    return (
        <div className="section card">
            <h3 className="subtitle">Create rules manually</h3>
            <div className="columns is-multiline">
                {rules.map(((rule, key) =>
                        <Rule rule={rule} ruleLength={ruleLength} key={key} onClick={onClick}/>
                ))}
            </div>
            <div className="row">
                <div className={"configItem"}>
                    <button className="button is-info" onClick={randomInitRules}>Random Init rules</button>
                </div>
                <div className={"configItem "}>
                    <button className="button is-danger" onClick={clearRules}>Clear rules</button>
                </div>
            </div>
        </div>
    )
}

export default ConfigRulesSection;
