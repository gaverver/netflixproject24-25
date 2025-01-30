import './App.css';
import { Routes, Route } from 'react-router-dom';
import DisconnectedHome from './pages/disconnected_home/Home';
import Register from './pages/registration/Register';
import NotFound from './pages/page404/notFound';
import Login from './pages/login/Login'


function App() {
  // router for all the pages in the application
  return (
    <div className="App">
      <Routes> 
        <Route path ="/home" element={<DisconnectedHome />} />
        <Route path ="/register" element={<Register />} />
        <Route path ="/login" element={<Login />} />
        <Route path ="/*" element={<NotFound />} />
      </Routes>
    </div>
  );
}
export default App;
