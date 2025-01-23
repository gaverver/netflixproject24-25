import "./input.css"
function Input({ className, autoComplete, type, placeHolder, value, onChange, required }) {
    return (
        <input 
            className={className} 
            autoComplete={autoComplete} 
            type={type} 
            placeholder={placeHolder} 
            value={value} 
            onChange={onChange} 
            required={required} 
        />
    );
}
export default Input