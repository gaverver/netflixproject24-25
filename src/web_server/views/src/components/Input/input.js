import "./input.css"
function Input({ text, className, type, placeHolder, value, onChange, required }) {
    return (
        <div>
            <label className="label-input"> {text} </label>
            <input 
                className={className} 
                type={type} 
                placeholder={placeHolder} 
                value={value} 
                onChange={onChange} 
                required={required} 
            />
        </div>
        
        
    );
}
export default Input