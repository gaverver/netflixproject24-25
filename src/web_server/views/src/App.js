import './App.css';
import { Routes, Route } from 'react-router-dom';
import DisconnectedHome from './pages/disconnected_home/Home';
import Admin from './pages/administration/Admin';
import MovieAdmin from './pages/administration/MovieAdmin';

function App() {
  // router for all the pages in the application
  return (
    <div className="App">
      <Routes> 
        <Route path ="/home" element={<DisconnectedHome />} />
        <Route path ="/admin" element={<Admin />} />
        <Route path="/admin/movies" element={<MovieAdmin/>}/>
      </Routes>
    </div>
  );
}
export default App;
