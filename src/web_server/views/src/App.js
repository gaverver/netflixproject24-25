import './App.css';
import './components/menu/menu'

function App() {
    return (
        <div>
            <Menu userName="gavri" photo='../public/images_for_decoration/gavri.jpg' isLoggedIn={true} isAdmin={true} />
        </div>
    );
}

export default App;
