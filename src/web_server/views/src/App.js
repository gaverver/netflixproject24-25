import './App.css';
import { Routes, Route } from 'react-router-dom';
import DisconnectedHome from './pages/disconnected_home/Home';


function App() {
  return (
    <div className="App">
      <Routes> 
        <Route path ="/home" element={<DisconnectedHome />} />
      </Routes>
    </div>
  );
}

export default App;
