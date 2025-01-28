import Input from '../Input/input'
import './RegisterLine.css'
function RegisterLine({typeFirst, typeSecond, placeHolderFirst, placeHolderSecond, valueFirst, valueSecond, onChangeFirst, onChangeSecond }) {
    return (
        <div className="register-line">
            <Input text={placeHolderFirst} className="register-field" type={typeFirst} placeHolder={placeHolderFirst} value={valueFirst} onChange={onChangeFirst} required="required"/>
            <Input text={placeHolderSecond} className="register-field" type={typeSecond} placeHolder={placeHolderSecond} value={valueSecond} onChange={onChangeSecond} required="required"/>
        </div>
    );
}


export default RegisterLine;