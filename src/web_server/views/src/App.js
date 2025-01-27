import './App.css';
import { Routes, Route } from 'react-router-dom';
import DisconnectedHome from './pages/disconnected_home/Home';
import SearchResults from './pages/searchResults/searchResults';


function App() {
  // router for all the pages in the application
  return (
    <div className="App">
      <Routes> 
        <Route path ="/home" element={<DisconnectedHome />} />
        <Route path="/movies/search/:query" element={<SearchResults />} />
      </Routes>
    </div>
  );
}


export default App;
